#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Blueprint/UserWidget.h"
#include "../BlockchainConnector.h"

#include <memory>
#include <vector>

#include "PostchainClient/common.h"
#include "PostchainClient/postchain_transaction.h"
#include "PostchainClient/GTX/gtx.h"
#include "PostchainClient/blockchain_client.h"
#include "FT3/Core/key_pair.h"
#include "FT3/Core/Blockchain/blockchain_session.h"

#include "AuthService.generated.h"

using namespace chromia;
using namespace chromia::postchain;
using namespace chromia::postchain::client;
using namespace chromia::postchain::ft3;

#define SSO_VAULT_URL "https://dev.vault.chromia-development.com"
#define SSO_PROTOCOL "alice"
#define SSO_SUCCESS_URL "alice://login"
#define SSO_CANCEL_URL "alice://cancel-login"

USTRUCT()
struct FQueryObjectPair
{
    GENERATED_BODY()

public:
    FQueryObjectPair() {};

    FQueryObjectPair(FString _name, FString _content) : m_Name(_name), m_Content(_content) {};

    FString m_Name;
    FString m_Content;
};

UCLASS()
class CHROMACLIENT_API AAuthService : public AActor {

	GENERATED_BODY()

public:
	AAuthService(const FObjectInitializer& ObjectInitializer);

	void Init(std::shared_ptr<ABlockchainConnector> blockchainConnector);

	std::shared_ptr<BlockchainSession> GetSession();

	std::shared_ptr<PlayerData> AuthenticateUserWithKey(FString loginKey, bool shouldNotifySubscribers = true);
    
    std::shared_ptr<PlayerData> GetPlayerDataByAccountID(FString accountId);
    
	FString Query(FString queryName, TArray<FQueryObjectPair> rawQueryObjects);

private:
   
	void CreateDappPlayer(FString accountId, FString username);

    std::shared_ptr<ABlockchainConnector> m_BlockchainConnector = nullptr;
	std::shared_ptr<BlockchainSession> m_Session = nullptr;

    FString m_AccountId;
};
