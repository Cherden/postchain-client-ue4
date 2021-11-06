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

	// Begin blueprint exposed functions
	UFUNCTION(BlueprintCallable)
	void Setup(FString blockchainRID, FString baseURL);

	UFUNCTION(BlueprintCallable)
	void SetMainWidget(UObject* mw);

	UFUNCTION(BlueprintCallable)
	void InitializeBRIDFromChainID();

	UFUNCTION(BlueprintCallable)
	void RegisterUser(FString username);

	UFUNCTION(BlueprintCallable)
	void CheckUser(FString username);
	// End blueprint exposed functions

	/**
	* Init GET request to wait for some POST transaction insert operation
	*/
	void WaitForBlockchainConfirmation();

	/**
	* Init new transaction from signers list
	*/
	TSharedPtr<Transaction> NewTransaction(TArray<TArray<byte>> signers);
	
	/*Called when the server has responded to InitializeBRIDFromChainID http request*/
	void OnBRIDResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	/*Called when the server has responded to RegisterUser http request*/
	void OnTransactionResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	/*Called when the server has responded to WaitForBlockchainConfirmation http request*/
	void OnBlockchainConfirmationReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	/*Called when the server has responded to CheckUser http request*/
	void OnQueryResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	/**
	* Add some message to the end of the on-screen console view. 
	* The message will be forwarded to main widget blueprint.
	*/
	void PrintLogOnScreen(FString message);


	//void OnHttpResponse(int status, std::string content);

private:
	FString BlockchainRID;
	int ChainID = 0;
	FString BaseURL;
	FString TxRID;

	TArray<byte> PrivateKey;
	TArray<byte> PublicKey;

	// Reference to owning Widget Blueprint
	UObject* MainWidget = nullptr;

	bool KeyPairIsValid();
};
