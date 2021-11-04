#include "BlockchainClient.h"
#include "Utils.h"
#include "../chroma-cpp-pure/src/postchain_util.h"

UBlockchainClient::UBlockchainClient(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
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


void UBlockchainClient::Setup(FString blockchainRID, FString baseURL)
{
	this->BlockchainRID = blockchainRID;
	this->BaseURL = baseURL;
}


TSharedPtr<Transaction> UBlockchainClient::NewTransaction(TArray<TArray<byte>> signers)
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


void UBlockchainClient::RegisterUser(FString username)
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

	Request->OnProcessRequestComplete().BindUObject(this, &UBlockchainClient::OnTransactionResponseReceived);

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


void UBlockchainClient::WaitForBlockchainConfirmation()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &UBlockchainClient::OnBlockchainConfirmationReceived);

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


void UBlockchainClient::InitializeBRIDFromChainID()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &UBlockchainClient::OnBRIDResponseReceived);

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


void UBlockchainClient::OnBRIDResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Display, TEXT("CHROMA::OnBRIDResponseReceived. Valid: %d"), bWasSuccessful);

	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Display, TEXT("CHROMA::OnBRIDResponseReceived %s"), *(Response->GetContentAsString()));
		this->BlockchainRID = Response->GetContentAsString();
	}
}


void UBlockchainClient::OnTransactionResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Display, TEXT("CHROMA::OnTransactionResponseReceived. Valid: %d"), bWasSuccessful);

	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Display, TEXT("CHROMA::OnTransactionResponseReceived %s"), *(Response->GetContentAsString()));
		//this->BlockchainRID = Response->GetContentAsString();
	}
}

void UBlockchainClient::OnBlockchainConfirmationReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Display, TEXT("CHROMA::OnBlockchainConfirmationReceived. Valid: %d"), bWasSuccessful);

	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Display, TEXT("CHROMA::OnBlockchainConfirmationReceived %s"), *(Response->GetContentAsString()));
		FString content = Response->GetContentAsString();
		if (content.Contains("waiting"))
		{
			WaitForBlockchainConfirmation();
		}
		//this->BlockchainRID = Response->GetContentAsString();
	}
}

bool UBlockchainClient::KeyPairIsValid()
{
	if (!(PrivateKey.Num() == 32)) return false;
	if (!(PublicKey.Num() == 33)) return false;
	return true;
}
