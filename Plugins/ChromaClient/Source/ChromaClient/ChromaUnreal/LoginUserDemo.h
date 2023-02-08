#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Auth/BlockchainConnector.h"
#include "Auth/Services/AuthService.h"
#include "Auth/Services/RequestService.h"
#include "Login/UserAccountManager.h"

//#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/BlueprintFunctionLibrary.h"
//#include "Blueprint/UserWidget.h"
#include <memory>
//#include <vector>
#include "LoginUserDemo.generated.h"

#define MOCK_USER_PRIVKEY "68484A224DC5A1C56B8B8C39A6FD5461BA4C3579506B3E85A6163350FD5E00CA"
#define DEFAULT_USER_NAME "DefaultUser"

using namespace chromia;
using namespace chromia::postchain;
using namespace chromia::postchain::client;

enum ELoginState
{
	eWaitingVaultAuthorization,
	eAddingAuthDescriptor,
	eCreatingPlayer,
	eEnteringGame,
	eAccountList,
	eFinalizeVaultLogin,
	eCreatePlayerPanel
};

UCLASS()
class CHROMACLIENT_API ALoginUserDemo : public AActor {

	GENERATED_BODY()

public:
	ALoginUserDemo(const FObjectInitializer& ObjectInitializer);

	~ALoginUserDemo();

	/* Override from parent class AActor */
	virtual void BeginPlay() override;

	/* Override from parent class AActor */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable)
	void Setup(FString blockchainRID, FString baseURL, FString privateKey);

	UFUNCTION(BlueprintCallable)
	TArray<USavedAccount*> GetLocalUserList();

	UFUNCTION(BlueprintCallable)
    TArray<FString> GetLocalUsernamesList();

	UFUNCTION(BlueprintCallable)
    bool CreatePlayer(FString username, FString key = "");
	
	UFUNCTION(BlueprintCallable)
	bool RemovePlayer(FString accountId);

	UFUNCTION(BlueprintCallable)
	bool GivePlayerTrash(FString accountId, FString tokenName);

	UFUNCTION(BlueprintCallable)
	FString GetPlayerInventory(FString accountId);

	void RenewLocalUserListOnNewChain();

	static AuthService* GetAuthService();

	static BlockchainConnector* GetBlockchainConnector();

	static RequestService* GetRequestService();

private:

    void EnterGame(std::shared_ptr<PlayerData> playerData, std::shared_ptr<KeyPair> keyPair);

	static BlockchainConnector* m_BlockchainConnector;
	static AuthService* m_AuthService;
	static RequestService* m_RequestService;

	FString m_BlockchainRID;
	FString m_BaseURL;
	FString m_PrivateKey;

	ELoginState m_loginUIState;
};
