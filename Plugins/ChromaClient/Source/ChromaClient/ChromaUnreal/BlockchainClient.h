#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Blueprint/UserWidget.h"

#include "PostchainClient/common.h"
#include "PostchainClient/postchain_transaction.h"
#include "PostchainClient/GTX/gtx.h"
#include "PostchainClient/blockchain_client.h"

#include <memory>
#include <vector>

#include "BlockChainClient.generated.h"

using namespace chromia;
using namespace chromia::postchain;
using namespace chromia::postchain::client;

UCLASS()
class CHROMACLIENT_API ABlockchainClient : public AActor {

	GENERATED_BODY()

public:
	ABlockchainClient(const FObjectInitializer& ObjectInitializer);

	// Begin blueprint exposed functions
	UFUNCTION(BlueprintCallable)
	void Setup(FString blockchainRID, FString baseURL);

	UFUNCTION(BlueprintCallable)
	void SetMainWidget(UObject* mw);

	UFUNCTION(BlueprintCallable)
	void RegisterUser(FString username);

	UFUNCTION(BlueprintCallable)
	void CheckUser(FString username);

	/**
	* Blocking query call 
	*/
	UFUNCTION(BlueprintCallable)
	FString Query(FString queryName, FString queryObjectName, FString queryObjectContent);

	/**
	* Blocking operation call
	*/
	UFUNCTION(BlueprintCallable)
	FString Operation(FString operationName, TArray<FString> operationValues);

	// End blueprint exposed functions

	/**
	* Add some message to the end of the on-screen console view. 
	* The message will be forwarded to main widget blueprint.
	*/
	void PrintLogOnScreen(FString message);

private:
	std::shared_ptr<BlockchainClient> BlockchainClientPtr;
	std::vector<BYTE> PrivateKey;
	std::vector<BYTE> PublicKey;

	// Reference to owning Widget Blueprint
	UObject* MainWidget = nullptr;

	bool KeyPairIsValid();
};
