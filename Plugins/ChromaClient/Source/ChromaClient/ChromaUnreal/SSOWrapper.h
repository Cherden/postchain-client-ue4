#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Blueprint/UserWidget.h"
#include <memory>
#include <vector>
#include "SSOWrapper.generated.h"

using namespace chromia;
using namespace chromia::postchain;
using namespace chromia::postchain::client;

UCLASS()
class CHROMACLIENT_API ASSOWrapper : public AActor {

	GENERATED_BODY()

public:
	ASSOWrapper(const FObjectInitializer& ObjectInitializer);

	// Begin blueprint exposed functions
	UFUNCTION(BlueprintCallable)
	void Login();

	UFUNCTION(BlueprintCallable)
	void SetMainWidget(UObject* mw);

	UFUNCTION(BlueprintCallable)
	void Setup(FString blockchainRID, FString baseURL);
	// End blueprint exposed functions

	/**
	* Add some message to the end of the on-screen console view. 
	* The message will be forwarded to main widget blueprint.
	*/
	void PrintLogOnScreen(FString message);

	// Get the state of login action, wich is executed async
	bool IsLoginInProgress();

private:
	FString BlockchainRID;
	FString BaseURL;

	// Reference to owning Widget Blueprint
	UObject* MainWidget = nullptr;

	// The state of login action, wich is executed async
	bool LoginInProgress = false;
};
