#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Blueprint/UserWidget.h"
#include "../BlockchainConnector.h"
#include "../../Structs/UtilStructs.h"

#include "PostchainClient/common.h"
#include "PostchainClient/postchain_transaction.h"
#include "PostchainClient/GTX/gtx.h"
#include "PostchainClient/blockchain_client.h"
#include "FT3/Core/key_pair.h"
#include "FT3/Core/Blockchain/blockchain_session.h"

#include <memory>
#include <vector>

using namespace chromia;
using namespace chromia::postchain;
using namespace chromia::postchain::client;
using namespace chromia::postchain::ft3;

#define SSO_VAULT_URL "https://dev.vault.chromia-development.com"
#define SSO_PROTOCOL "alice"
#define SSO_SUCCESS_URL "alice://login"
#define SSO_CANCEL_URL "alice://cancel-login"

class AuthService {

public:
    AuthService();

    ~AuthService();

	void Init();

	std::shared_ptr<BlockchainSession> GetSession();

	std::shared_ptr<PlayerData> AuthenticateUserWithKey(FString loginKey, bool shouldNotifySubscribers = true);
    
    std::shared_ptr<PlayerData> GetPlayerDataByAccountID(FString accountId);
    
    std::shared_ptr<PlayerData> CreateMockFt3User(std::shared_ptr<KeyPair> localKeypair);

    bool RegisterNewPlayer(FString accountId, FString username, std::shared_ptr<PlayerData> &outPlayerData, std::shared_ptr<User> &outUser);

	FString Query(FString queryName, TArray<FQueryObjectPair> rawQueryObjects);

private:
   
	bool CreateDappPlayer(FString accountId, FString username);

	std::shared_ptr<BlockchainSession> m_Session = nullptr;

    FString m_AccountId;
};
