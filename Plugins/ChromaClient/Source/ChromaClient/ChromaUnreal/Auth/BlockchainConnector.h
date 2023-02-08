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

#define REQUEST_TIMEOUT_MILLIS  10000
#define LOOP_DELAY_MILLIS	    500

using namespace chromia;
using namespace chromia::postchain;
using namespace chromia::postchain::client;
using namespace chromia::postchain::ft3;

class PlayerData
{
public:
	FString m_Id;
	FString m_Username;
	FString m_Tokens;
	FString m_DateOfBirth;
};

class BlockchainConnector{

public:
	BlockchainConnector();

	~BlockchainConnector();

	void LoadBlockchainConfig(FString path);

	void InitializeBlockchain(FString blockchainRID, FString baseURL);

	std::shared_ptr<BlockchainSession> CreateSession(FString privKey, TArray<FlagsType> flags = {});

	std::shared_ptr<Blockchain> GetBlockchain();

	bool IsInitialized();

private:
	FString m_BlockchainRID;
	FString m_BlockchainUrl;
	std::shared_ptr<KeyPair> m_KeyPair;
	std::shared_ptr<Blockchain> m_Blockchain;
	bool m_BlockchainIsInitialized = false;
};
