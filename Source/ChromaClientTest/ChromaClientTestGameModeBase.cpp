
#include "ChromaClientTestGameModeBase.h"

void AChromaClientTestGameModeBase::BeginPlay()
{
	UE_LOG(LogTemp, Display, TEXT("CHROMA::AChromaClientTestGameModeBase::BeginPlay()"));

	FString blockchainRID("CDB5F8CC52E39272EF2178B87DE9A6D629636F52FD97ED5D13C1EB3BD299E3E2");
	FString baseURL("https://rellide-staging.chromia.dev/node/15327");

	BlockchainClient = NewObject<UBlockchainClient>();	
	BlockchainClient->Setup(blockchainRID, baseURL);

	BlockchainClient->InitializeBRIDFromChainID();

	BlockchainClient->RegisterUser("qwerty");
}
