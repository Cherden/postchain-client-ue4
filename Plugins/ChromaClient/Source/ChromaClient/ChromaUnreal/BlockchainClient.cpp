#include "BlockchainClient.h"
#include "Engine/GameEngine.h"
#include "Misc/OutputDeviceDebug.h"
#include "Utils.h"
#include "../chroma-cpp-pure/src/PostchainClient/postchain_util.h"
#include "../chroma-cpp-pure/src/HTTP/httprequest.h"


ABlockchainClient::ABlockchainClient(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	if (!PostchainUtil::GenerateKeyPair(PrivateKey, PublicKey))
	{
		UE_LOG(LogTemp, Error, TEXT("CHROMA::UBlockchainClient::GenerateKeyPair failed"));
	}

	BlockchainClientPtr = std::make_shared<BlockchainClient>();
}


void ABlockchainClient::SetMainWidget(UObject* mw)
{
	this->MainWidget = mw;
}


void ABlockchainClient::Setup(FString blockchainRID, FString baseURL)
{
	BlockchainClientPtr->Setup(ChromaUtils::FStringToSTDString(blockchainRID), ChromaUtils::FStringToSTDString(baseURL));
}


void ABlockchainClient::RegisterUser(FString username)
{
	if (!KeyPairIsValid())
	{
		PrintLogOnScreen("RegisterUser fail. Missing KeyPair");
		return;
	}
	std::shared_ptr<PostchainTransaction> transaction = BlockchainClientPtr->NewTransaction(std::vector<std::vector<byte>> { PublicKey }, [this](std::string error) {
		PrintLogOnScreen(FString("Transaction failed: ")  + ChromaUtils::STDStringToFString(error));
	});

	std::shared_ptr<gtv::ArrayValue> operation_values = AbstractValueFactory::EmptyArray();
	operation_values->Add(AbstractValueFactory::Build(ChromaUtils::FStringToSTDString(username)));
	transaction->AddOperation(std::string("register_user"), operation_values);

	///*
	//	The blockchain hashes the operations to generate a transaction id (txid) which has to be unique.
	//	In order to prevent this colision, a "nop" operation can be attached which will be filtered by
	//	the blockchain.
	//*/
	std::shared_ptr<gtv::ArrayValue> nop_operation_values = AbstractValueFactory::EmptyArray();
	static int nonce = PostchainUtil::RandomIntInRange(0, 100000);
	nop_operation_values->Add(AbstractValueFactory::Build(std::to_string(nonce)));
	transaction->AddOperation(std::string("nop"), nop_operation_values);

	transaction->Sign(PrivateKey, PublicKey);

	transaction->PostAndWait([this, username] (std::string content) {
		PrintLogOnScreen("User " + username + " registered successfully.");
	});

	return;
}


void ABlockchainClient::CheckUser(FString username)
{
	std::vector<QueryObject> queryObjects;
	queryObjects.push_back(QueryObject("name", AbstractValueFactory::Build(ChromaUtils::FStringToSTDString(username))));

	BlockchainClientPtr->Query(
		"check_user",
		queryObjects, 
		[this, username] (std::string content) {
			FString result = content.compare("1") == 0 ? "true" : "false";
			PrintLogOnScreen("check_user: " + username + "  " + result);
		},
		[this](std::string error) {
			PrintLogOnScreen(FString("check_user failed, error: ") + ChromaUtils::STDStringToFString(error));
		}
	);
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
	if (!(PrivateKey.size() == 32)) return false;
	if (!(PublicKey.size() == 33)) return false;
	return true;
}
