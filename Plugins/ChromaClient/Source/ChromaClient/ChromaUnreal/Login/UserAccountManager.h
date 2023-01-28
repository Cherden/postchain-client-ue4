#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "../Auth/BlockchainConnector.h"
#include "../Structs/UtilStructs.h"

#include "PostchainClient/common.h"
#include "PostchainClient/postchain_transaction.h"
#include "PostchainClient/GTX/gtx.h"
#include "PostchainClient/blockchain_client.h"
#include "FT3/Core/key_pair.h"
#include "FT3/Core/Blockchain/blockchain_session.h"

#include <memory>
#include <vector>

#define USER_ACCOUNT_MANAGER_FILENAME "players.dat"

using namespace chromia;
using namespace chromia::postchain;
using namespace chromia::postchain::client;
using namespace chromia::postchain::ft3;

class UserAccountManager{

public:
	UserAccountManager();

	~UserAccountManager();

    static void EnterGameWithUser(std::shared_ptr<PlayerData> playerData);

    static bool AddNewUserAndSaveLocal(FString accountId, FString username, std::shared_ptr<KeyPair> keypair);

    static bool RemoveUserAndSaveLocal(FString accountId);

    static bool RemoveLocalUsersIfChainIsNew();

    static bool SaveLocalUsers();

    static bool LoadLocalUsers();

    static void SetAccountId(FString id);

	static TArray<USavedAccount*> GetLocalUsers();

private:
	static FString m_AccountId;
	static TArray<USavedAccount*> m_LocalUsers;
};
