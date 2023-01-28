#include "UserAccountManager.h"
#include "../LoginUserDemo.h"
#include "../ChromaUtils.h"

#include "Engine/GameEngine.h"
#include "Misc/OutputDeviceDebug.h"

#include "PostchainClient/postchain_util.h"
#include "HTTP/httprequest.h"
#include "FT3/Core/key_pair.h"
#include "FT3/Core/chain_connection_info.h"
#include "FT3/Core/Blockchain/directory_service.h"
#include "FT3/Core/Blockchain/blockchain.h"
#include "FT3/User/account.h"
#include "FT3/User/user.h"
#include "FT3/User/AuthDescriptor/single_signature_auth_descriptor.h"
#include "SSO/file_manager.h"

#include "TestUtil/directory_service_util.h"

FString UserAccountManager::m_AccountId;
TArray<USavedAccount*> UserAccountManager::m_LocalUsers;

UserAccountManager::UserAccountManager()
{
   
}

UserAccountManager::~UserAccountManager()
{
    UE_LOG(LogTemp, Display, TEXT("CHROMA::UserAccountManager::~UserAccountManager()"));
}

void UserAccountManager::EnterGameWithUser(std::shared_ptr<PlayerData> playerData)
{
    m_AccountId = playerData->m_Id;
    UE_LOG(LogTemp, Display, TEXT("UserAccountManager::EnterGameWithUser"));
}

bool UserAccountManager::AddNewUserAndSaveLocal(FString accountId, FString username, std::shared_ptr<KeyPair> keypair)
{
    string privKey = PostchainUtil::ByteVectorToHexString(keypair->priv_key_);

    USavedAccount *newUser = NewObject<USavedAccount>();
    newUser->m_AccountId = accountId;
    newUser->m_Username = username;
    newUser->m_PrivKey = ChromaUtils::STDStringToFString(privKey);

    // Create new entry
    m_LocalUsers.Add(newUser);

    return SaveLocalUsers();
}

bool UserAccountManager::RemoveUserAndSaveLocal(FString accountId)
{
    for (size_t i = 0u; i < m_LocalUsers.Num(); i++)
    {
        if (m_LocalUsers[i]->m_AccountId == accountId)
        {
            m_LocalUsers.RemoveAt(i);
            return SaveLocalUsers();
            break;
        }
    }

    // User was not found
    return false;
}

bool UserAccountManager::RemoveLocalUsersIfChainIsNew()
{
    AuthService* authService = ALoginUserDemo::GetAuthService();

    FString playersDataStr = authService->Query(
        "admin.list_players", 
        { 
            FQueryObjectPair("search", ""),
            FQueryObjectPair("page_size", 1),
            FQueryObjectPair("page_num", 0),
        });

    if (playersDataStr.Len() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("CHROMA::UserAccountManager::Remove LocalUsersIfChainIsNew Query(admin.list_players, ...) failed"));
        return false;
    }

    nlohmann::json json_obj = nlohmann::json::parse(ChromaUtils::FStringToSTDString(playersDataStr));
    int totalPlayers = PostchainUtil::GetSafeJSONInt(json_obj, std::string("total"));
    // A fresh chain will have only admin account (Alice)
    if (totalPlayers <= 1)
    {
        m_LocalUsers.Empty();
        SaveLocalUsers();
        return true;
    }

    return false;
}

bool UserAccountManager::SaveLocalUsers()
{
    // Serialize
    nlohmann::json json_obj;
    json_obj["users"] = nlohmann::json::array();

    for (size_t i=0; i < m_LocalUsers.Num(); i++)
    {
        USavedAccount* account = m_LocalUsers[i];
        nlohmann::json user_json_obj;
        user_json_obj["username"] = ChromaUtils::FStringToSTDString(account->m_Username);
        user_json_obj["privkey"] = ChromaUtils::FStringToSTDString(account->m_PrivKey);
        user_json_obj["accountid"] = ChromaUtils::FStringToSTDString(account->m_AccountId);
        json_obj["users"].push_back(user_json_obj);
    }
   
    std::string dataStr = json_obj.dump();
    UE_LOG(LogTemp, Display, TEXT("CHROMA::UserAccountManager::SaveLocalUsers serialized data: %s"), *ChromaUtils::STDStringToFString(dataStr));

    // Store in file
    if (FileManager::WriteToFile(USER_ACCOUNT_MANAGER_FILENAME, dataStr))
    {
        UE_LOG(LogTemp, Display, TEXT("CHROMA::UserAccountManager::SaveLocalUsers saved to: %s"), TEXT(USER_ACCOUNT_MANAGER_FILENAME));
        return true;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("CHROMA::UserAccountManager::SaveLocalUsers failed to save"));
        return false;
    }
}

bool UserAccountManager::LoadLocalUsers()
{
    m_LocalUsers.Empty();

    std::string dataStr = "";
    FileManager::LoadFromFile(USER_ACCOUNT_MANAGER_FILENAME, dataStr);

    if (!FileManager::LoadFromFile(USER_ACCOUNT_MANAGER_FILENAME, dataStr))
    {
        UE_LOG(LogTemp, Warning, TEXT("CHROMA::UserAccountManager::LoadLocalUsers failed to read from %s"), TEXT(USER_ACCOUNT_MANAGER_FILENAME));
        return false;
    }

    if (dataStr.size() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("CHROMA::UserAccountManager::LoadLocalUsers dataStr.size() == 0"));
        return false;
    }   

    nlohmann::json json_obj = nlohmann::json::parse(dataStr);
    if (!json_obj.contains("users"))
    {
        UE_LOG(LogTemp, Warning, TEXT("CHROMA::UserAccountManager::LoadLocalUsers !json_obj.contains[users]"));
        return false;
    }

    for (size_t i = 0; i < json_obj["users"].size(); i++)
    {
        nlohmann::json user_json_obj = json_obj["users"][i];
        USavedAccount* user = NewObject<USavedAccount>();
        user->m_Username = ChromaUtils::STDStringToFString(PostchainUtil::GetSafeJSONString(user_json_obj, "username"));
        user->m_PrivKey = ChromaUtils::STDStringToFString(PostchainUtil::GetSafeJSONString(user_json_obj, "privkey"));
        user->m_AccountId = ChromaUtils::STDStringToFString(PostchainUtil::GetSafeJSONString(user_json_obj, "accountid"));
        m_LocalUsers.Add(user);
    }

    return true;
}

void UserAccountManager::SetAccountId(FString id)
{
    m_AccountId = id;
}

TArray<USavedAccount*> UserAccountManager::GetLocalUsers()
{
    return m_LocalUsers;
}