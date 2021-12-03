#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Blueprint/UserWidget.h"

#include "../chroma-cpp-pure/src/common.h"
#include "../chroma-cpp-pure/src/postchain_transaction.h"
#include "../chroma-cpp-pure/src/GTX/gtx.h"
#include "../chroma-cpp-pure/src/blockchain_client.h"

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
	// End blueprint exposed functions

	/**
	* Add some message to the end of the on-screen console view. 
	* The message will be forwarded to main widget blueprint.
	*/
	void PrintLogOnScreen(FString message);

private:
	std::shared_ptr<BlockchainClient> BlockchainClientPtr;
	std::vector<byte> PrivateKey;
	std::vector<byte> PublicKey;

	// Reference to owning Widget Blueprint
	UObject* MainWidget = nullptr;

	bool KeyPairIsValid();
};
