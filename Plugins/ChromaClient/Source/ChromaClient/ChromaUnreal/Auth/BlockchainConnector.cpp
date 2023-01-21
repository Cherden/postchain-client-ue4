#include "BlockchainConnector.h"
#include "Engine/GameEngine.h"
#include "Misc/OutputDeviceDebug.h"
#include "../ChromaUtils.h"

#include "PostchainClient/postchain_util.h"
#include "HTTP/httprequest.h"
#include "FT3/Core/key_pair.h"
#include "FT3/Core/chain_connection_info.h"
#include "FT3/Core/Blockchain/directory_service.h"
#include "FT3/Core/Blockchain/blockchain.h"
#include "FT3/User/account.h"
#include "FT3/User/user.h"
#include "FT3/User/AuthDescriptor/single_signature_auth_descriptor.h"

#include "TestUtil/directory_service_util.h"

ABlockchainConnector::ABlockchainConnector(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    m_BlockchainIsInitialized = false;
	LoadBlockchainConfig("blockchain-config");
}

void ABlockchainConnector::LoadBlockchainConfig(FString path)
{
    try
    {
        //TODO Read from json
        m_BlockchainRID = "D48072CF435596AAFDE950365AFAEB8083A3083DFDFD5B96E80FACA62503B1FA";
        m_BlockchainUrl = "http://localhost:7740/";
    }
    catch (std::exception &e)
    {
        UE_LOG(LogTemp, Error, TEXT("CHROMA::ABlockchainConnector::LoadBlockchainConfig failed : %s"),
            *ChromaUtils::STDStringToFString(e.what()));
    }
}



void ABlockchainConnector::InitializeBlockchain()
{
    std::function<void(std::shared_ptr<Blockchain>)> on_success = [&](std::shared_ptr<Blockchain> blockchain) {
        m_Blockchain = blockchain;
        m_BlockchainIsInitialized = true;
    };

    std::function<void(std::string)> on_error = [&](std::string error) {
        UE_LOG(LogTemp, Error, TEXT("CHROMA::ABlockchainConnector::InitializeBlockchain failed : %s"),
            *ChromaUtils::STDStringToFString(error));
    };

    Blockchain::Initialize(
        ChromaUtils::FStringToSTDString(m_BlockchainRID),
        DirectoryServiceUtil::GetDefaultDirectoryService(ChromaUtils::FStringToSTDString(m_BlockchainRID),
                                                         ChromaUtils::FStringToSTDString(m_BlockchainUrl)),
        on_success,
        on_error
    );
}

std::shared_ptr<BlockchainSession> ABlockchainConnector::CreateSession(FString privKey, TArray<FlagsType> flags)
{
    // Wait for blockchain to be initialized
    long delay = 0;
    while (!m_BlockchainIsInitialized)
    {
        PostchainUtil::SleepForMillis(LOOP_DELAY_MILLIS);
        delay += LOOP_DELAY_MILLIS; 
        if (delay >= REQUEST_TIMEOUT_MILLIS)
        {
            UE_LOG(LogTemp, Error, TEXT("CHROMA::ABlockchainConnector::CreateSession timeout"));
            return nullptr;
        }
    }

    if (flags.Num() == 0) 
    { 
        flags.Add(FlagsType::eTransfer);
    }

    m_KeyPair = std::make_shared<KeyPair>(ChromaUtils::FStringToSTDString(privKey));

    std::shared_ptr<AuthDescriptor> authDescriptor = std::make_shared<SingleSignatureAuthDescriptor>(
        m_KeyPair->pub_key_, ChromaUtils::GenericTArrayToSTDArray<FlagsType>(flags));
    std::shared_ptr<User> user = std::make_shared<User>(m_KeyPair, authDescriptor);

    std::shared_ptr<BlockchainSession> blockchainSession = std::make_shared<BlockchainSession>(user, m_Blockchain);

    return blockchainSession;
}