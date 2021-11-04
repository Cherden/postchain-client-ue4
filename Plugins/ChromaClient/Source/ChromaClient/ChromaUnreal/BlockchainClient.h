#pragma once

#include "CoreMinimal.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/BlueprintFunctionLibrary.h"

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
class CHROMACLIENT_API UBlockchainClient : public UBlueprintFunctionLibrary {

	GENERATED_BODY()

public:
	UBlockchainClient(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	void Setup(FString blockchainRID, FString baseURL);

	TSharedPtr<Transaction> NewTransaction(TArray<TArray<byte>> signers);

	UFUNCTION()
	void InitializeBRIDFromChainID();

	void RegisterUser(FString username);

	void WaitForBlockchainConfirmation();

	/*Called when the server has responded to InitializeBRIDFromChainID http request*/
	void OnBRIDResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void OnTransactionResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void OnBlockchainConfirmationReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

private:
	FString BlockchainRID;
	int ChainID = 0;
	FString BaseURL;
	FString TxRID;

	TArray<byte> PrivateKey;
	TArray<byte> PublicKey;

	bool KeyPairIsValid();
};
