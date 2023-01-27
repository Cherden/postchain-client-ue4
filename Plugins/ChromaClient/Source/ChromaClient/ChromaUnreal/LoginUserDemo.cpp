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

std::shared_ptr<UBlockchainConnector> ALoginUserDemo::m_blockchainConnector;
std::shared_ptr<UAuthService> ALoginUserDemo::m_AuthService;
std::shared_ptr<URequestService> ALoginUserDemo::m_RequestService;


ALoginUserDemo::ALoginUserDemo(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void ALoginUserDemo::BeginPlay()
{
   
}

void ALoginUserDemo::Setup(FString blockchainRID, FString baseURL, FString privateKey)
{
	this->m_BlockchainRID = blockchainRID;
	this->m_BaseURL = baseURL;
	this->m_PrivateKey = privateKey;

    m_blockchainConnector.reset(NewObject<UBlockchainConnector>());
    m_blockchainConnector->InitializeBlockchain(this->m_BlockchainRID, this->m_BaseURL);

    // TODO make services async, this is just a demo
    m_AuthService.reset(NewObject<UAuthService>());
    m_AuthService->Init(m_blockchainConnector);

    m_RequestService.reset(NewObject<URequestService>());
    m_RequestService->Init(m_AuthService);

    std::shared_ptr<PlayerData> playerData = m_AuthService->AuthenticateUserWithKey(m_PrivateKey);
    if (playerData != nullptr)
    {
        EnterGame(playerData, std::make_shared<KeyPair>(ChromaUtils::FStringToSTDString(m_PrivateKey)));
    }
}
//
//TArray<FSavedAccount*> ALoginUserDemo::GetLocalUserList()
//{
//    UUserAccountManager::LoadLocalUsers();
//    //return UUserAccountManager::GetLocalUsers();
//    return TArray<FSavedAccount*>();
//}

TArray<USavedAccountClass*> ALoginUserDemo::GetLocalUserListClass()
{
    return TArray<USavedAccountClass*>();
}

void ALoginUserDemo::RenewLocalUserListOnNewChain()
{
    bool chainWasReset = UUserAccountManager::RemoveLocalUsersIfChainIsNew();
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
        if (m_AuthService->RegisterNewPlayer(playerData->m_Id, "qqq5", newPlayerData, newUser))
        {
            UUserAccountManager::AddNewUserAndSaveLocal(newPlayerData->m_Id, newPlayerData->m_Username, newUser->key_pair_);
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

std::shared_ptr<UAuthService> ALoginUserDemo::GetAuthService()
{
    return m_AuthService;
}

std::shared_ptr<UBlockchainConnector> ALoginUserDemo::GetBlockchainConnector()
{
    return m_blockchainConnector;
}

std::shared_ptr<URequestService> ALoginUserDemo::GetRequestService()
{
    return m_RequestService;
}

void ALoginUserDemo::EnterGame(std::shared_ptr<PlayerData> playerData, std::shared_ptr<KeyPair> keyPair)
{
    if (playerData->m_Username.Len() == 0)
    {
        // Fresh account, need registration
        m_loginUIState = ELoginState::eCreatePlayerPanel;
        UUserAccountManager::SetAccountId(playerData->m_Id);
        //SceneManager.LoadScene("Game");
    }
    else
    {
        // Old account login on a new machine
        m_loginUIState = ELoginState::eAddingAuthDescriptor;
        UUserAccountManager::AddNewUserAndSaveLocal(playerData->m_Id, playerData->m_Username, keyPair);
        UUserAccountManager::EnterGameWithUser(playerData);
    }
}
