#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
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
class CHROMACLIENT_API URequestService : public UObject {

	GENERATED_BODY()

public:
	URequestService(const FObjectInitializer& ObjectInitializer);

	void Init(std::shared_ptr<UAuthService> authService);

    bool Call(std::vector<std::shared_ptr<Operation>> operations);

private:

	void CallOperationsSync(std::vector<std::shared_ptr<Operation>> operations, std::function<void()> onSuccess, std::function<void(std::string)> onError);

    std::shared_ptr<UAuthService> m_AuthService;
};
