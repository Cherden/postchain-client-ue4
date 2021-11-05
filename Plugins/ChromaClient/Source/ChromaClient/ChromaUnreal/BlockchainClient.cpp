
#include "BlockchainClient.h"
#include "Engine/GameEngine.h"
#include "Misc/OutputDeviceDebug.h"

#include "Utils.h"
#include "../chroma-cpp-pure/src/postchain_util.h"

ABlockchainClient::ABlockchainClient(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	std::vector<unsigned char> private_key;
	std::vector<unsigned char> public_key;
	if (!PostchainUtil::GenerateKeyPair(private_key, public_key))
	{
		UE_LOG(LogTemp, Error, TEXT("CHROMA::UBlockchainClient::GenerateKeyPair failed"));
	}
	else 
	{
		UE_LOG(LogTemp, Display, TEXT("CHROMA::UBlockchainClient::privatekey: [%s]  publickey: [%s]"), 
			*(ChromaUtils::STDStringToFString(PostchainUtil::KeyToString(private_key))),
			*(ChromaUtils::STDStringToFString(PostchainUtil::KeyToString(public_key)))
		);

		PrivateKey = ChromaUtils::STDArrayToTArray(private_key);
		PublicKey = ChromaUtils::STDArrayToTArray(public_key);
	}
}


void ABlockchainClient::SetMainWidget(UUserWidget* mw)
{
	this->MainWidget = mw;
	int y = 0;
	y++;
	y++;

	const FString command = FString::Printf(TEXT("SetConsoleTest %s"), TEXT("Blockchain"));
	FOutputDeviceDebug debug;
	MainWidget->CallFunctionByNameWithArguments(*command, debug, this, true);

}


void ABlockchainClient::Setup(FString blockchainRID, FString baseURL)
{
	this->BlockchainRID = blockchainRID;
	this->BaseURL = baseURL;
}


TSharedPtr<Transaction> ABlockchainClient::NewTransaction(TArray<TArray<byte>> signers)
{
	std::shared_ptr<Gtx> gtx = std::make_shared<Gtx>(ChromaUtils::FStringToSTDString(this->BlockchainRID));

	for(TArray<byte> &signer : signers)
	{
		std::vector<byte> arr = ChromaUtils::TArrayToSTDArray(signer);
		gtx->AddSignerToGtx(arr);
	}

	TSharedPtr<Transaction> transaction = MakeShared<Transaction>(gtx, ChromaUtils::FStringToSTDString(this->BaseURL), ChromaUtils::FStringToSTDString(this->BlockchainRID));
	return transaction;
}


void ABlockchainClient::RegisterUser(FString username)
{
	if (!KeyPairIsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("CHROMA::RegisterUser fail. Missing KeyPair"));
		return;
	}
	TSharedPtr<Transaction> transaction = NewTransaction({PublicKey});

	std::shared_ptr<gtv::ArrayValue> operation_values = AbstractValueFactory::EmptyArray();
	operation_values->Add(AbstractValueFactory::Build(ChromaUtils::FStringToSTDString(username)));
	transaction->AddOperation(std::string("register_user"), operation_values);

	///*
	//	The blockchain hashes the operations to generate a transaction id (txid) which has to be unique.
	//	In order to prevent this colision, a "nop" operation can be attached which will be filtered by
	//	the blockchain.
	//*/
	std::shared_ptr<gtv::ArrayValue> nop_operation_values = AbstractValueFactory::EmptyArray();
	nop_operation_values->Add(AbstractValueFactory::Build("123"));  //TO-DO randomize
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
	//std::shared_ptr<Query> query = std::make_shared<Query>(this->BlockchainRID, this->BaseURL);

	std::vector<QueryObject> query_objects;
	query_objects.push_back(QueryObject("name", AbstractValueFactory::Build(ChromaUtils::FStringToSTDString(username))));

	std::string json = PostchainUtil::QueryToJSONString("check_user", query_objects);
	FString payload = ChromaUtils::STDStringToFString(json);

	UE_LOG(LogTemp, Display, TEXT("CHROMA::Query json: [%s]"), *payload);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &ABlockchainClient::OnQueryResponseReceived);

	FString url = FString::Printf(TEXT("%s/query/%s"), *(this->BaseURL), *(this->BlockchainRID));
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

	//this._urlBase, "query/" + this._brid);

;	/*var request = new PostchainQuery<T>(this._baseURL, this._blockchainRID);

	yield return request.Query(queryName, queryObject);

	if (request.error)
	{
		onError(request.errorMessage);
	}
	else
	{
		onSuccess(request.content);
	}*/
}


void ABlockchainClient::WaitForBlockchainConfirmation()
{

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &ABlockchainClient::OnBlockchainConfirmationReceived);

	FString url = FString::Printf(TEXT("%s/tx/%s/%s/status"), *(this->BaseURL), *(this->BlockchainRID), *(this->TxRID));

	Request->SetURL(*url);

	UE_LOG(LogTemp, Display, TEXT("CHROMA::URL : %s"), *url);

	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");

	//Request->SetHeader("Content-Type", "application/json");

	//Send the request
	Request->ProcessRequest();

	UE_LOG(LogTemp, Display, TEXT("CHROMA::WaitForBlockchainConfirmation ProcessRequest();"));
}


void ABlockchainClient::InitializeBRIDFromChainID()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &ABlockchainClient::OnBRIDResponseReceived);

	FString url = FString::Printf(TEXT("%s/brid/iid_%d"), *(this->BaseURL), this->ChainID);
	//Request->SetURL(TEXT("http://rellide-staging.chromia.dev/node/15927/brid/iid_0"));
	Request->SetURL(*url);

	UE_LOG(LogTemp, Display, TEXT("CHROMA::URL : %s"), *url);

	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");

	//Request->SetHeader("Content-Type", "application/json");
	
	//Send the request
	Request->ProcessRequest();

	UE_LOG(LogTemp, Display, TEXT("CHROMA::ProcessRequest();"));
}


void ABlockchainClient::OnBRIDResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Display, TEXT("CHROMA::OnBRIDResponseReceived. Valid: %d"), bWasSuccessful);

	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Display, TEXT("CHROMA::OnBRIDResponseReceived %s"), *(Response->GetContentAsString()));
		PrintLogOnScreen(*(Response->GetContentAsString()));

		this->BlockchainRID = Response->GetContentAsString();
	}
}


void ABlockchainClient::OnTransactionResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Display, TEXT("CHROMA::OnTransactionResponseReceived. Valid: %d"), bWasSuccessful);

	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Display, TEXT("CHROMA::OnTransactionResponseReceived %s"), *(Response->GetContentAsString()));
		PrintLogOnScreen(*(Response->GetContentAsString()));
	}
}

void ABlockchainClient::OnBlockchainConfirmationReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Display, TEXT("CHROMA::OnBlockchainConfirmationReceived. Valid: %d"), bWasSuccessful);

	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Display, TEXT("CHROMA::OnBlockchainConfirmationReceived %s"), *(Response->GetContentAsString()));
		PrintLogOnScreen(*(Response->GetContentAsString()));

		FString content = Response->GetContentAsString();
		if (content.Contains("waiting"))
		{
			WaitForBlockchainConfirmation();
		}
	}
}

void ABlockchainClient::OnQueryResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	//UE_LOG(LogTemp, Display, TEXT("CHROMA::OnQueryResponseReceived. Valid: %d"), bWasSuccessful);

	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Display, TEXT("CHROMA::OnQueryResponseReceived %s"), *(Response->GetContentAsString()));
		PrintLogOnScreen(*(Response->GetContentAsString()));

		/*if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, *(Response->GetContentAsString()));
		}*/
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
