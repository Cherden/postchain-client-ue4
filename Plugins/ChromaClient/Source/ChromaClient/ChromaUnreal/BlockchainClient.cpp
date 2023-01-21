#include "BlockchainClient.h"
#include "Engine/GameEngine.h"
#include "Misc/OutputDeviceDebug.h"
#include "ChromaUtils.h"

#include "PostchainClient/postchain_util.h"
#include "HTTP/httprequest.h"
#include "FT3/Core/key_pair.h"

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


FString ABlockchainClient::Query(FString queryName, FString queryObjectName, FString queryObjectContent)
{
	std::vector<QueryObject> queryObjects;
	queryObjects.push_back(QueryObject(
		ChromaUtils::FStringToSTDString(queryObjectName),
		AbstractValueFactory::Build(ChromaUtils::FStringToSTDString(queryObjectContent))));

	FString result = "";

	BlockchainClientPtr->Query(
		ChromaUtils::FStringToSTDString(queryName),
		queryObjects,
		[this, &result](std::string content) {
			result = ChromaUtils::STDStringToFString(content);
			PrintLogOnScreen(FString("Query succeeded, content: ") + ChromaUtils::STDStringToFString(content));
		},
		[this, &result](std::string error) {
			result = ChromaUtils::STDStringToFString(error);
			PrintLogOnScreen(FString("Query failed, error: ") + ChromaUtils::STDStringToFString(error));
		}
	);

	return result;
}

FString ABlockchainClient::Operation(FString operationName, TArray<FString> operationValues)
{
	FString result = "";

	std::shared_ptr<PostchainTransaction> transaction = BlockchainClientPtr->NewTransaction(std::vector<std::vector<byte>> { PublicKey }, [this](std::string error) {
		PrintLogOnScreen(FString("Transaction failed: ") + ChromaUtils::STDStringToFString(error));
	});

	std::shared_ptr<gtv::ArrayValue> operation_values = AbstractValueFactory::EmptyArray();
	for (int i = 0; i < operationValues.Num(); i++)
	{
		operation_values->Add(AbstractValueFactory::Build(ChromaUtils::FStringToSTDString(operationValues[i])));
	}
	transaction->AddOperation(ChromaUtils::FStringToSTDString(operationName), operation_values);

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

	transaction->PostAndWait([&](std::string content) {
		result = ChromaUtils::STDStringToFString(content);
		PrintLogOnScreen(FString("Transaction succeeded, content: ") + ChromaUtils::STDStringToFString(content));
	});

	return result;
}

using namespace chromia::postchain::ft3;
//#include <string>

void ABlockchainClient::CreateEditorTestUser()
{
	PrintLogOnScreen(FString("CreateEditorTestUser()"));

	//LoginUIState = LoginState.CreatingPlayer;

	std::string mockUserPrivKey = "68484A224DC5A1C56B8B8C39A6FD5461BA4C3579506B3E85A6163350FD5E00CA";
	KeyPair tmpKeyPairForLogi(mockUserPrivKey);

	//var playerData = await _authService.CreateMockFt3User(tmpKeyPairForLogin);

	/*try
	{
		var playerData = await _authService.CreateMockFt3User(tmpKeyPairForLogin);
		if (playerData.username == null)
		{
			if (key != null)
			{
				(PlayerData playerData2, User user) = await _authService.RegisterNewPlayer(playerData.id, "Editor Test User");
				UserAccountManager.AddNewUserAndSaveLocal(playerData2.id, playerData2.username, user.KeyPair);
				LoginUIState = LoginState.AccountList;
				RefreshLocalUserList();
			}
			else
			{
				(PlayerData playerData2, User user) = await _authService.RegisterNewPlayer(playerData.id, "Editor Test User");
				UserAccountManager.AddNewUserAndSaveLocal(playerData2.id, playerData2.username, user.KeyPair);
				LoginUIState = LoginState.AccountList;
				RefreshLocalUserList();
			}
		}
	}
	catch (Exception e)
	{
		Debug.LogWarning(e);
	}
	finally
	{
		LoginUIState = LoginState.AccountList;
	}*/

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
