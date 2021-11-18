
#include "BlockchainClient.h"
#include "Engine/GameEngine.h"
#include "Misc/OutputDeviceDebug.h"

#include "Utils.h"
#include "../chroma-cpp-pure/src/postchain_util.h"

#include "../chroma-cpp-pure/tests/FT3/account_test.h"

ABlockchainClient::ABlockchainClient(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

	AccountTest account_test;
	bool pass = account_test.AccountTest2();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AccountTest2: [%d]"), pass);

	//this->BlockchainRID = "1711E1936B67292DB3D6E72DFBB8969DB12F199E2E6D75F9138011A3343FAB95";
	//this->BaseURL = "https://rellide-staging.chromia.dev/node/16283/";

	std::vector<unsigned char> private_key;
	std::vector<unsigned char> public_key;
	if (!PostchainUtil::GenerateKeyPair(private_key, public_key))
	{
		UE_LOG(LogTemp, Error, TEXT("CHROMA::UBlockchainClient::GenerateKeyPair failed"));
	}
	else 
	{
		PrivateKey = ChromaUtils::STDArrayToTArray(private_key);
		PublicKey = ChromaUtils::STDArrayToTArray(public_key);
	}

	
	//RegisterUser("abcdef");
}


void ABlockchainClient::SetMainWidget(UObject* mw)
{
	this->MainWidget = mw;
}


void ABlockchainClient::Setup(FString blockchainRID, FString baseURL)
{
	this->BlockchainRID = blockchainRID;
	this->BaseURL = baseURL;
}


TSharedPtr<PostchainTransaction> ABlockchainClient::NewTransaction(TArray<TArray<byte>> signers)
{
	std::shared_ptr<Gtx> gtx = std::make_shared<Gtx>(ChromaUtils::FStringToSTDString(this->BlockchainRID));

	for(TArray<byte> &signer : signers)
	{
		std::vector<byte> arr = ChromaUtils::TArrayToSTDArray(signer);
		gtx->AddSignerToGtx(arr);
	}

	TSharedPtr<PostchainTransaction> transaction = MakeShared<PostchainTransaction>(
		gtx, 
		ChromaUtils::FStringToSTDString(this->BaseURL), 
		ChromaUtils::FStringToSTDString(this->BlockchainRID),
		[](std::string error) {});
	return transaction;
}


void ABlockchainClient::RegisterUser(FString username)
{
	if (!KeyPairIsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("CHROMA::RegisterUser fail. Missing KeyPair"));
		return;
	}
	TSharedPtr<PostchainTransaction> transaction = NewTransaction({PublicKey});

	std::shared_ptr<gtv::ArrayValue> operation_values = AbstractValueFactory::EmptyArray();
	operation_values->Add(AbstractValueFactory::Build(ChromaUtils::FStringToSTDString(username)));
	transaction->AddOperation(std::string("register_user"), operation_values);

	///*
	//	The blockchain hashes the operations to generate a transaction id (txid) which has to be unique.
	//	In order to prevent this colision, a "nop" operation can be attached which will be filtered by
	//	the blockchain.
	//*/
	std::shared_ptr<gtv::ArrayValue> nop_operation_values = AbstractValueFactory::EmptyArray();
	static int nonce = 123; // PostchainUtil::RandomIntInRange(0, 100000);
	UE_LOG(LogTemp, Display, TEXT("CHROMA transaction nop nonce: %d"), nonce);
	nop_operation_values->Add(AbstractValueFactory::Build(std::to_string(nonce)));
	transaction->AddOperation(std::string("nop"), nop_operation_values);

	transaction->Sign(ChromaUtils::TArrayToSTDArray(PrivateKey), ChromaUtils::TArrayToSTDArray(PublicKey));

	TxRID = ChromaUtils::STDStringToFString(transaction->GetTxRID());

	// -------------------------- POST -------------------------
	
	//var payload = String.Format(@"{{""tx"": ""{0}""}}", Serialize());

	std::string stdSerialized = transaction->Serialize();
	FString serialized = ChromaUtils::STDStringToFString(stdSerialized);

	FString payload = FString::Printf(TEXT("{\"tx\": \"%s\"}"), *serialized);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &ABlockchainClient::OnTransactionResponseReceived);

	FString url = FString::Printf(TEXT("%s/tx/%s"), *(this->BaseURL), *(this->BlockchainRID));
	UE_LOG(LogTemp, Display, TEXT("CHROMA::URL : [%s]"), *url);
	UE_LOG(LogTemp, Display, TEXT("CHROMA::Payload Length : [%d]"), payload.Len());
	UE_LOG(LogTemp, Display, TEXT("CHROMA::Payload : [%s]"), *payload);

	Request->SetURL(*url);

	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	//Request->SetHeader(TEXT("Accepts"), TEXT("application/json"));

	Request->SetContentAsString(*payload);
	//Request->SetHeader("Content-Type", "application/json");

	//Send the request
	Request->ProcessRequest();

	UE_LOG(LogTemp, Display, TEXT("CHROMA::ProcessRequest();"));

	WaitForBlockchainConfirmation();

	return;
}


void ABlockchainClient::CheckUser(FString username)
{
	//--------------------------------------------------------------------------
	// Prepare query object
	std::vector<QueryObject> queryObjects;
	queryObjects.push_back(QueryObject("name", AbstractValueFactory::Build(ChromaUtils::FStringToSTDString(username))));
	//---------------------------------------------------------------------------
	
	//---------------------------------------------------------------------------
	// Send POST request
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();

	// Reqister callback
	Request->OnProcessRequestComplete().BindUObject(this, &ABlockchainClient::OnQueryResponseReceived);

	FString url = FString::Printf(TEXT("%s/query/%s"), *(this->BaseURL), *(this->BlockchainRID));
	Request->SetURL(*url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	// Add request content as json
	std::string json = PostchainUtil::QueryToJSONString("check_user", queryObjects);
	FString payload = ChromaUtils::STDStringToFString(json);
	Request->SetContentAsString(*payload);

	// Send the request
	Request->ProcessRequest();
	//---------------------------------------------------------------------------
}


void ABlockchainClient::WaitForBlockchainConfirmation()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();

	// Reqister callback
	Request->OnProcessRequestComplete().BindUObject(this, &ABlockchainClient::OnBlockchainConfirmationReceived);

	FString url = FString::Printf(TEXT("%s/tx/%s/%s/status"), *(this->BaseURL), *(this->BlockchainRID), *(this->TxRID));

	Request->SetURL(*url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");

	//Send the request
	Request->ProcessRequest();
}


void ABlockchainClient::InitializeBRIDFromChainID()
{
	/*std::function<void(int, std::string)> callback = [](int error, std::string content) {
		UE_LOG(LogTemp, Error, TEXT("CHROMA::onSuccess lambda called: %d  %s"), error, *(ChromaUtils::STDStringToFString(content)));
	};

	FString url = FString::Printf(TEXT("%s/brid/iid_%d"), *(this->BaseURL), this->ChainID);
	chromia::postchain::http::HttpRequest request(ChromaUtils::FStringToSTDString(url), callback);
	request.Get();*/

	// HTTPS
	//FString url = FString::Printf(TEXT("%s/brid/iid_%d"), *(this->BaseURL), this->ChainID);
	
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();

	// Reqister callback
	Request->OnProcessRequestComplete().BindUObject(this, &ABlockchainClient::OnBRIDResponseReceived);

	FString url = FString::Printf(TEXT("%s/brid/iid_%d"), *(this->BaseURL), this->ChainID);
	Request->SetURL(*url);

	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");

	//Send the request
	Request->ProcessRequest();
}


void ABlockchainClient::OnBRIDResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Display, TEXT("CHROMA::OnBRIDResponseReceived. Valid: %d"), bWasSuccessful);

	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Display, TEXT("CHROMA::OnBRIDResponseReceived %s"), *(Response->GetContentAsString()));
		this->BlockchainRID = Response->GetContentAsString();
	}
}


void ABlockchainClient::OnTransactionResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Display, TEXT("CHROMA::OnTransactionResponseReceived. Valid: %d"), bWasSuccessful);

	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Display, TEXT("CHROMA::OnTransactionResponseReceived %s"), *(Response->GetContentAsString()));
		PrintLogOnScreen("Transaction sent");
	}
}

void ABlockchainClient::OnBlockchainConfirmationReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Display, TEXT("CHROMA::OnBlockchainConfirmationReceived. Valid: %d"), bWasSuccessful);

	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Display, TEXT("CHROMA::OnBlockchainConfirmationReceived %s"), *(Response->GetContentAsString()));

		FString content = Response->GetContentAsString();
		if (content.StartsWith("{")) // Check for json content
		{
			nlohmann::json jsonContent = nlohmann::json::parse(ChromaUtils::FStringToSTDString(content));

			if (jsonContent.contains("status")) {
				std::string stdStatus = jsonContent["status"];
				FString status = ChromaUtils::STDStringToFString(stdStatus);
				if (status.Equals("waiting"))
				{
					WaitForBlockchainConfirmation();
				}
				else if (status.Equals("confirmed"))
				{
					PrintLogOnScreen(TEXT("User registered successfully!"));
				}
				else
				{
					PrintLogOnScreen(FString::Printf(TEXT("Failed to register user. status: %s"), *status));
				}
			}
			else {
				PrintLogOnScreen(TEXT("Invalid blockchain response"));
			}
		}
		else 
		{
			PrintLogOnScreen(TEXT("Invalid response blockchain response, check connection."));
		}
	}
}


void ABlockchainClient::OnQueryResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		FString response = Response->GetContentAsString();

		if (response.Equals("1"))
		{
			PrintLogOnScreen(TEXT("User is registered"));
		}
		else 
		{
			PrintLogOnScreen(TEXT("User is not registered"));
		}
	}
	else 
	{
		PrintLogOnScreen(TEXT("Invalid query response"));
	}
}


void ABlockchainClient::PrintLogOnScreen(FString message)
{
	if (MainWidget == nullptr) return;

	const FString command = FString::Printf(TEXT("PrintLog %s"), *message);
	FOutputDeviceDebug debug;
	MainWidget->CallFunctionByNameWithArguments(*command, debug, this, true);
}


bool ABlockchainClient::KeyPairIsValid()
{
	if (!(PrivateKey.Num() == 32)) return false;
	if (!(PublicKey.Num() == 33)) return false;
	return true;
}
