#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Blueprint/UserWidget.h"
#include "../BlockchainConnector.h"
#include "AuthService.h"

#include <memory>
#include <vector>

#include "PostchainClient/common.h"
#include "PostchainClient/postchain_transaction.h"
#include "PostchainClient/GTX/gtx.h"
#include "PostchainClient/blockchain_client.h"
#include "FT3/Core/key_pair.h"
#include "FT3/Core/Blockchain/blockchain_session.h"

#include "RequestService.generated.h"

using namespace chromia;
using namespace chromia::postchain;
using namespace chromia::postchain::client;
using namespace chromia::postchain::ft3;

UCLASS()
class CHROMACLIENT_API ARequestService : public AActor {

	GENERATED_BODY()

public:
	ARequestService(const FObjectInitializer& ObjectInitializer);

	void Init(std::shared_ptr<AAuthService> authService);

   // void Call(TArray<Operation> operations);

private:
	//void CallOperationsSync(TArray<Operation> operations, std::function<void(std::string)> on_error, std::function<void(std::string)> onError);

    std::shared_ptr<AAuthService> m_AuthService;
};
