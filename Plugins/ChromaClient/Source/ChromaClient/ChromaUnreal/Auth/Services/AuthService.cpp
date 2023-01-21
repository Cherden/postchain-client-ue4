#include "AuthService.h"
#include "SSO/protocol_handler.h"
#include "SSO/sso.h"
#include "../../ChromaUtils.h"

AAuthService::AAuthService(const FObjectInitializer& ObjectInitializer)
{

}

void AAuthService::Init(std::shared_ptr<ABlockchainConnector> blockchainConnector)
{
    m_BlockchainConnector = blockchainConnector;

    // TODO implement this function
    ProtocolHandler::HandleTempTx(SSO_PROTOCOL);

    SSO::SetVaultUrl(SSO_VAULT_URL);
}

std::shared_ptr<PlayerData> AAuthService::AuthenticateUserWithKey(FString loginKey, bool shouldNotifySubscribers)
{
    std::shared_ptr<KeyPair> keyPair = std::make_shared<KeyPair>(ChromaUtils::FStringToSTDString(loginKey));

    if (m_BlockchainConnector == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("CHROMA::AAuthService::AuthenticateUserWithKey failed, BlockchainConnector is nullptr"));
        return nullptr;
    }

    m_Session = m_BlockchainConnector->CreateSession(loginKey);
    std::shared_ptr<User> user = m_Session->user_;
    std::vector<std::shared_ptr<Account>> accounts;

    m_Session->GetAccountsByAuthDescriptorId(user->auth_descriptor_->ID(), 
        [&](std::vector<std::shared_ptr<Account>> _accounts) {
            accounts = _accounts;
        },
        [&](std::string error) {
            UE_LOG(LogTemp, Error, TEXT("CHROMA::ABlockchainConnector::InitializeBlockchain failed : %s"),
                *ChromaUtils::STDStringToFString(error));
        }
    );

    if (accounts.size() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("CHROMA::AAuthService::AuthenticateUserWithKey failed, accounts.size() == 0"));
        return nullptr;
    }

    std::shared_ptr<PlayerData> playerData = GetPlayerDataByAccountID(ChromaUtils::STDStringToFString(accounts[0]->GetID()));

    m_AccountId = ChromaUtils::STDStringToFString(accounts[0]->GetID());

    return playerData;
}

std::shared_ptr<PlayerData> AAuthService::GetPlayerDataByAccountID(FString accountId)
{
    FString playerDataStr = Query("player.find_by_account_id", { FQueryObjectPair("account_id", accountId) });

    if (playerDataStr.Len() == 0)
    {
        return nullptr;
    }
    
    std::shared_ptr<PlayerData> playerData = std::make_shared<PlayerData>();

    // TODO check this json parsing
    nlohmann::json json_content = nlohmann::json::parse(ChromaUtils::FStringToSTDString(playerDataStr));
    playerData->m_Id = ChromaUtils::STDStringToFString(PostchainUtil::GetSafeJSONString(json_content, std::string("id")));
    playerData->m_Username = ChromaUtils::STDStringToFString(PostchainUtil::GetSafeJSONString(json_content, std::string("username")));
    playerData->m_Tokens = ChromaUtils::STDStringToFString(PostchainUtil::GetSafeJSONString(json_content, std::string("tokens")));
    playerData->m_DateOfBirth = ChromaUtils::STDStringToFString(PostchainUtil::GetSafeJSONString(json_content, std::string("dateofbirth")));

    return playerData;
}

std::shared_ptr<BlockchainSession> AAuthService::GetSession()
{
    return m_Session;
}

FString AAuthService::Query(FString queryName, TArray<FQueryObjectPair> rawQueryObjects)
{
    std::vector<QueryObject> queryObjects;
    for (size_t i = 0; i < rawQueryObjects.Num(); i++)
    {
        queryObjects.push_back(QueryObject(
            ChromaUtils::FStringToSTDString(rawQueryObjects[i].m_Name),
            AbstractValueFactory::Build(ChromaUtils::FStringToSTDString(rawQueryObjects[i].m_Content))));
    }

    FString result = "";
    auto blockchain = m_BlockchainConnector->GetBlockchain();
    blockchain->Query(
        ChromaUtils::FStringToSTDString(queryName),
        queryObjects,
        [&result](std::string content) {
            result = ChromaUtils::STDStringToFString(content);
           
        },
        [&result](std::string error) {
            result = "";
            UE_LOG(LogTemp, Error, TEXT("CHROMA::AAuthService::Query failed : %s"), *ChromaUtils::STDStringToFString(error));
        }
    );

    return result;
}


void AAuthService::CreateDappPlayer(FString accountId, FString username)
{
    //try
    //{
    //    var requestService = ServiceLocator<IRequestService>.Get();
    //    object[] args = { username, accountId, session.User.AuthDescriptor.ID };

    //    await requestService.Call(new Chromia.Postchain.Ft3.Operation("player.new", args));
    //}
    //catch (RequestErrorException e)
    //{
    //    if (createPlayerVALErrorMessages.ContainsKey(e.Issue))
    //    {
    //        // throw new RequestErrorException(
    //        //     e.Type,
    //        //     e.Issue,
    //        //     e,
    //        //     _localizationService.Localize(LocalizationType.Generic, createPlayerVALErrorMessages[e.Issue])
    //        // );
    //    }
    //    else throw;
    //}
}
