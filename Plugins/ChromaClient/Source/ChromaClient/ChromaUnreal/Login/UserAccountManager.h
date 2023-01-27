#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "../Auth/BlockchainConnector.h"

#include <memory>
#include <vector>

#include "PostchainClient/common.h"
#include "PostchainClient/postchain_transaction.h"
#include "PostchainClient/GTX/gtx.h"
#include "PostchainClient/blockchain_client.h"
#include "FT3/Core/key_pair.h"
#include "FT3/Core/Blockchain/blockchain_session.h"

#include "UserAccountManager.generated.h"

#define USER_ACCOUNT_MANAGER_FILENAME "players.dat"

using namespace chromia;
using namespace chromia::postchain;
using namespace chromia::postchain::client;
using namespace chromia::postchain::ft3;

UCLASS()
class USavedAccount : public UObject
{
	GENERATED_BODY()

public:
	USavedAccount(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {};

	UPROPERTY(BlueprintReadOnly)
		FString m_Username;

	UPROPERTY(BlueprintReadOnly)
		FString m_PrivKey;

	UPROPERTY(BlueprintReadOnly)
		FString m_AccountId;
};

UCLASS()
class CHROMACLIENT_API UUserAccountManager : public UObject {

	GENERATED_BODY()

public:
	UUserAccountManager(const FObjectInitializer& ObjectInitializer);

    static void EnterGameWithUser(std::shared_ptr<PlayerData> playerData);

    static void AddNewUserAndSaveLocal(FString accountId, FString username, std::shared_ptr<KeyPair> keypair);

    static void RemoveUserAndSaveLocal(FString accountId);

    static bool RemoveLocalUsersIfChainIsNew();

    static void SaveLocalUsers();

    static void LoadLocalUsers();

    static void SetAccountId(FString id);

	static TArray<USavedAccount*> GetLocalUsers();

private:
	static FString m_AccountId;
	static TArray<USavedAccount*> m_LocalUsers;
};
