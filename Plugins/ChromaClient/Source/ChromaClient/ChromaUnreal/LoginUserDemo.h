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

	/* Override from parent class AActor */
	virtual void BeginPlay() override;

	/* Override from parent class AActor */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable)
	void Setup(FString blockchainRID, FString baseURL, FString privateKey);

	UFUNCTION(BlueprintCallable)
	TArray<USavedAccount*> GetLocalUserList();

    void RenewLocalUserListOnNewChain();

	UFUNCTION(BlueprintCallable)
    void CreateEditorTestUser(FString key = "");

	/*static std::shared_ptr<UAuthService> GetAuthService();

	static std::shared_ptr<UBlockchainConnector> GetBlockchainConnector();

	static std::shared_ptr<URequestService> GetRequestService();*/

	static UAuthService* GetAuthService();

	static UBlockchainConnector* GetBlockchainConnector();

	static URequestService* GetRequestService();

private:

    void EnterGame(std::shared_ptr<PlayerData> playerData, std::shared_ptr<KeyPair> keyPair);

	// TODO maybe delete rid and baseUrl
	FString m_BlockchainRID;
	FString m_BaseURL;
	FString m_PrivateKey;

	static UBlockchainConnector* m_blockchainConnector;
	static UAuthService* m_AuthService;
	static URequestService* m_RequestService;

	/*static std::shared_ptr<UBlockchainConnector> m_blockchainConnector;
	static std::shared_ptr<UAuthService> m_AuthService;
	static std::shared_ptr<URequestService> m_RequestService;*/

	ELoginState m_loginUIState;
};
