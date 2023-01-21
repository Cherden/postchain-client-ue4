#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Blueprint/UserWidget.h"

#include <memory>
#include <vector>

#include "PostchainClient/common.h"
#include "PostchainClient/postchain_transaction.h"
#include "PostchainClient/GTX/gtx.h"
#include "PostchainClient/blockchain_client.h"
#include "FT3/Core/key_pair.h"
#include "FT3/Core/Blockchain/blockchain_session.h"

#include "BlockchainConnector.generated.h"

#define REQUEST_TIMEOUT_MILLIS  10000
#define LOOP_DELAY_MILLIS	    500

using namespace chromia;
using namespace chromia::postchain;
using namespace chromia::postchain::client;
using namespace chromia::postchain::ft3;

UCLASS()
class CHROMACLIENT_API ABlockchainConnector : public AActor {

	GENERATED_BODY()

public:
	ABlockchainConnector(const FObjectInitializer& ObjectInitializer);

	void LoadBlockchainConfig(FString path);

	void InitializeBlockchain();

	std::shared_ptr<BlockchainSession> CreateSession(FString privKey, TArray<FlagsType> flags = {});

private:
	FString m_BlockchainRID;
	FString m_BlockchainUrl;
	std::shared_ptr<KeyPair> m_KeyPair;
	
	std::shared_ptr<Blockchain> m_Blockchain;
	bool m_BlockchainIsInitialized = false;
};
