#include "LoginUserDemo.h"

#include "ChromaUtils.h"

#include "Engine/GameEngine.h"
#include "Misc/OutputDeviceDebug.h"
#include "Async/Async.h"

#include "PostchainClient/postchain_util.h"
#include "SSO/sso.h"
#include "SSO/file_manager.h"
#include "FT3/Core/postchain.h"
#include "FT3/Core/Blockchain/blockchain.h"

#include "Auth/Services/AuthService.h"

using namespace chromia::postchain::ft3;

BlockchainConnector* ALoginUserDemo::m_BlockchainConnector = nullptr;
AuthService* ALoginUserDemo::m_AuthService = nullptr;
RequestService* ALoginUserDemo::m_RequestService = nullptr;

ALoginUserDemo::ALoginUserDemo(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
   
}

ALoginUserDemo::~ALoginUserDemo()
{
    UE_LOG(LogTemp, Error, TEXT("CHROMA::ALoginUserDemo::~ALoginUserDemo()"));
    if (m_BlockchainConnector != nullptr)
    {
        delete m_BlockchainConnector;
        m_BlockchainConnector = nullptr;
    }

    if (m_AuthService != nullptr)
    {
        delete m_AuthService;
        m_AuthService = nullptr;
    }

    if (m_RequestService != nullptr)
    {
        delete m_RequestService;
        m_RequestService = nullptr;
    }
}

void ALoginUserDemo::BeginPlay()
{
    UE_LOG(LogTemp, Display, TEXT("CHROMA::ALoginUserDemo::BeginPlay"));
}

void ALoginUserDemo::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    UE_LOG(LogTemp, Display, TEXT("CHROMA::ALoginUserDemo::EndPlay"));
}

void ALoginUserDemo::Setup(FString blockchainRID, FString baseURL, FString privateKey)
{
    // TODO make services async, this is just a demo

	this->m_BlockchainRID = blockchainRID;
	this->m_BaseURL = baseURL;
	this->m_PrivateKey = privateKey;

    m_BlockchainConnector = new BlockchainConnector();
    m_BlockchainConnector->InitializeBlockchain(this->m_BlockchainRID, this->m_BaseURL);

    // TODO make services async, this is just a demo
    m_AuthService = new AuthService();
    m_AuthService->Init();

    m_RequestService = new RequestService();

    std::shared_ptr<PlayerData> playerData = m_AuthService->AuthenticateUserWithKey(m_PrivateKey);
    if (playerData != nullptr)
    {
        EnterGame(playerData, std::make_shared<KeyPair>(ChromaUtils::FStringToSTDString(m_PrivateKey)));
    }

    RenewLocalUserListOnNewChain();
    UserAccountManager::LoadLocalUsers();
}

TArray<USavedAccount*> ALoginUserDemo::GetLocalUserList()
{
    UserAccountManager::LoadLocalUsers();
    return UserAccountManager::GetLocalUsers();
}

TArray<FString> ALoginUserDemo::GetLocalUsernamesList()
{
    UserAccountManager::LoadLocalUsers();
    TArray<USavedAccount*> localUsers = UserAccountManager::GetLocalUsers();

    TArray<FString> output;
    for (size_t i = 0u; i < localUsers.Num(); i++)
    {
        output.Add(FString(localUsers[i]->m_Username));
    }
    return output;
}

void ALoginUserDemo::RenewLocalUserListOnNewChain()
{
    bool chainWasReset = UserAccountManager::RemoveLocalUsersIfChainIsNew();
    if (chainWasReset)
    {
        CreateEditorTestUser(MOCK_USER_PRIVKEY);
    }
}

void ALoginUserDemo::CreateEditorTestUser(FString key)
{
    m_loginUIState = ELoginState::eCreatingPlayer;

    std::shared_ptr<KeyPair> tmpKeyPairForLogin = key.Len() == 0 ? std::make_shared<KeyPair>(ChromaUtils::FStringToSTDString(key)) : std::make_shared<KeyPair>();

    std::shared_ptr<PlayerData> playerData = m_AuthService->CreateMockFt3User(tmpKeyPairForLogin);

    if (playerData->m_Username.Len() == 0)
    {
        std::shared_ptr<PlayerData> newPlayerData;
        std::shared_ptr<User> newUser;
        if (m_AuthService->RegisterNewPlayer(playerData->m_Id, "qqq6", newPlayerData, newUser))
        {
            UserAccountManager::AddNewUserAndSaveLocal(newPlayerData->m_Id, newPlayerData->m_Username, newUser->key_pair_);
            UE_LOG(LogTemp, Display, TEXT("CHROMA::ALoginUserDemo::CreateEditorTestUser success"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("CHROMA::ALoginUserDemo::CreateEditorTestUser RegisterNewPlayer failed"));
        }
        // TODO refresh UI
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("CHROMA::ALoginUserDemo::CreateEditorTestUser failed"));
    }

    m_loginUIState = ELoginState::eAccountList;
}

AuthService* ALoginUserDemo::GetAuthService()
{
    return  m_AuthService;
}

BlockchainConnector* ALoginUserDemo::GetBlockchainConnector()
{
    return m_BlockchainConnector;
}

RequestService* ALoginUserDemo::GetRequestService()
{
    return m_RequestService;
}

void ALoginUserDemo::EnterGame(std::shared_ptr<PlayerData> playerData, std::shared_ptr<KeyPair> keyPair)
{
    if (playerData->m_Username.Len() == 0)
    {
        // Fresh account, need registration
        m_loginUIState = ELoginState::eCreatePlayerPanel;
        UserAccountManager::SetAccountId(playerData->m_Id);
        //SceneManager.LoadScene("Game");
    }
    else
    {
        // Old account login on a new machine
        m_loginUIState = ELoginState::eAddingAuthDescriptor;
        UserAccountManager::AddNewUserAndSaveLocal(playerData->m_Id, playerData->m_Username, keyPair);
        UserAccountManager::EnterGameWithUser(playerData);
    }
}
