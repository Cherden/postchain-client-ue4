#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Blueprint/UserWidget.h"

#include "../chroma-cpp-pure/src/common.h"
#include "../chroma-cpp-pure/src/transaction.h"
#include "../chroma-cpp-pure/src/GTX/gtx.h"

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

	// Begin blueprint exposed

	UFUNCTION(BlueprintCallable)
	void Setup(FString blockchainRID, FString baseURL);

	UFUNCTION(BlueprintCallable)
	void InitializeBRIDFromChainID();

	UFUNCTION(BlueprintCallable)
	void RegisterUser(FString username);

	UFUNCTION(BlueprintCallable)
	void CheckUser(FString username);
	
	// End blueprint exposed

	void WaitForBlockchainConfirmation();

	TSharedPtr<Transaction> NewTransaction(TArray<TArray<byte>> signers);
	
	/*Called when the server has responded to InitializeBRIDFromChainID http request*/
	void OnBRIDResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void OnTransactionResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void OnBlockchainConfirmationReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void OnQueryResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	void SetMainWidget(UObject* mw)
	{
		this->MainWidget = mw;
	}

	UObject* MainWidget;

private:
	FString BlockchainRID;
	int ChainID = 0;
	FString BaseURL;
	FString TxRID;

	TArray<byte> PrivateKey;
	TArray<byte> PublicKey;

	bool KeyPairIsValid();

	
};
