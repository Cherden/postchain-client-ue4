#include "UserAccountManager.h"

#include "../LoginUserDemo.h"

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

FString UUserAccountManager::m_AccountId;
TArray<FSavedAccount> UUserAccountManager::m_LocalUsers;

UUserAccountManager::UUserAccountManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
   
}

void UUserAccountManager::EnterGameWithUser(std::shared_ptr<PlayerData> playerData)
{
    m_AccountId = playerData->m_Id;
    UE_LOG(LogTemp, Display, TEXT("UUserAccountManager::EnterGameWithUser"));
}

void UUserAccountManager::AddNewUserAndSaveLocal(FString accountId, FString username, std::shared_ptr<KeyPair> keypair)
{
    string privKey = PostchainUtil::ByteVectorToHexString(keypair->priv_key_);

    FSavedAccount newUser;
    newUser.m_AccountId = accountId;
    newUser.m_Username = username;
    newUser.m_PrivKey = ChromaUtils::STDStringToFString(privKey);

    FSavedAccount* foundEntry = m_LocalUsers.FindByPredicate([accountId](const FSavedAccount& InItem)
    {
        return InItem.m_AccountId == accountId;
    });

    bool entryWasUpdated = false;
    for (size_t i = 0u; i < m_LocalUsers.Num(); i++)
    {
        if (m_LocalUsers[i].m_AccountId == accountId)
        {
            m_LocalUsers[i] = newUser;
            entryWasUpdated = true;
        }
    }

    if (!entryWasUpdated)
    {
        // Create new entry
        m_LocalUsers.Add(newUser);
    }
   
    SaveLocalUsers();
}

void UUserAccountManager::RemoveUserAndSaveLocal(FString accountId)
{
    for (size_t i = 0u; i < m_LocalUsers.Num(); i++)
    {
        if (m_LocalUsers[i].m_AccountId == accountId)
        {
            m_LocalUsers.RemoveAt(i);
            SaveLocalUsers();
            break;
        }
    }
}

bool UUserAccountManager::RemoveLocalUsersIfChainIsNew()
{
    std::shared_ptr<UAuthService> authService = ALoginUserDemo::GetAuthService();

    // TODO adjust this

    FString playersDataStr = authService->Query(
        "admin.list_players", 
        { 
            FQueryObjectPair("search", ""),
            FQueryObjectPair("page_size", 1),
            FQueryObjectPair("page_num", 0),
        });

    if (playersDataStr.Len() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("CHROMA::UUserAccountManager::RemoveLocalUsersIfChainIsNew Query(admin.list_players, ...) failed"));
        return false;
    }

    return true;
}

void UUserAccountManager::SaveLocalUsers()
{
    // TODO

   /* BinaryFormatter bf = new BinaryFormatter();
    using (FileStream file = File.Open(Application.persistentDataPath + SAVE_FILENAME, FileMode.OpenOrCreate))
    {
        bf.Serialize(file, LocalUsers);
    }

    Debug.Log("Player accounts saved to " + Application.persistentDataPath + SAVE_FILENAME);*/
}

void UUserAccountManager::LoadLocalUsers()
{
    // TODO

  /*  if (File.Exists(Application.persistentDataPath + SAVE_FILENAME))
    {
        try
        {
            BinaryFormatter bf = new BinaryFormatter();
            using (FileStream file = File.Open(Application.persistentDataPath + SAVE_FILENAME, FileMode.Open))
            {
                LocalUsers = (List<SavedAccount>)bf.Deserialize(file);
            }
        }
        catch (Exception e)
        {
            Debug.LogError(e);
            LocalUsers = new List<SavedAccount>();
        }
    }
    else
    {
        LocalUsers = new List<SavedAccount>();
    }*/
}

void UUserAccountManager::SetAccountId(FString id)
{
    m_AccountId = id;
}

TArray<FSavedAccount> UUserAccountManager::GetLocalUsers()
{
    return m_LocalUsers;
}