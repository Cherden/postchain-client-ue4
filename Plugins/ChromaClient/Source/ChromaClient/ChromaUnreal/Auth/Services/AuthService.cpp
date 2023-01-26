#include "AuthService.h"
#include "SSO/protocol_handler.h"
#include "SSO/sso.h"
#include "../../ChromaUtils.h"
#include "../../LoginUserDemo.h"

#include "FT3/User/AuthDescriptor/single_signature_auth_descriptor.h"
#include "FT3/Core/transaction_builder.h"
#include "FT3/Core/transaction.h"
#include "FT3/Core/operation.h"
#include "FT3/User/user.h"

UAuthService::UAuthService(const FObjectInitializer& ObjectInitializer)
{

}

void UAuthService::Init(std::shared_ptr<UBlockchainConnector> blockchainConnector)
{
    m_BlockchainConnector = blockchainConnector;

    // TODO implement this function
    ProtocolHandler::HandleTempTx(SSO_PROTOCOL);

    SSO::SetVaultUrl(SSO_VAULT_URL);
}

std::shared_ptr<BlockchainSession> UAuthService::GetSession()
{
    return m_Session;
}

std::shared_ptr<PlayerData> UAuthService::AuthenticateUserWithKey(FString loginKey, bool shouldNotifySubscribers)
{
    std::shared_ptr<KeyPair> keyPair = std::make_shared<KeyPair>(ChromaUtils::FStringToSTDString(loginKey));

    if (m_BlockchainConnector == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("CHROMA::UAuthService::AuthenticateUserWithKey failed, BlockchainConnector is nullptr"));
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
            UE_LOG(LogTemp, Error, TEXT("CHROMA::UAuthService::InitializeBlockchain failed : %s"),
                *ChromaUtils::STDStringToFString(error));
        }
    );

    if (accounts.size() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("CHROMA::UAuthService::AuthenticateUserWithKey failed, accounts.size() == 0"));
        return nullptr;
    }

    std::shared_ptr<PlayerData> playerData = GetPlayerDataByAccountID(ChromaUtils::STDStringToFString(accounts[0]->GetID()));

    m_AccountId = ChromaUtils::STDStringToFString(accounts[0]->GetID());

    return playerData;
}

std::shared_ptr<PlayerData> UAuthService::GetPlayerDataByAccountID(FString accountId)
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

std::shared_ptr<PlayerData> UAuthService::CreateMockFt3User(std::shared_ptr<KeyPair> localKeypair)
{
    std::vector<FlagsType> flags = { FlagsType::eTransfer };
    std::shared_ptr<SingleSignatureAuthDescriptor> primaryAuthDescriptor = std::make_shared<SingleSignatureAuthDescriptor>(
        localKeypair->pub_key_, flags);

    std::shared_ptr<User> user = std::make_shared<User>(localKeypair, primaryAuthDescriptor);
    std::shared_ptr<Blockchain> blockchain = ALoginUserDemo::GetBlockchainConnector()->GetBlockchain();
  
    std::shared_ptr<TransactionBuilder> tx_builder = blockchain->NewTransactionBuilder();
    std::shared_ptr<ArrayValue> op_args = AbstractValueFactory::EmptyArray();
    op_args->Add(primaryAuthDescriptor->ToGTV());
    tx_builder->Add(Operation::Op("ft3.dev_register_account", op_args));
    
    bool successfully = false;
    std::shared_ptr<Transaction> tx = tx_builder->BuildAndSign(user, [&successfully](std::string error) { 
        successfully = false; 
        UE_LOG(LogTemp, Error, TEXT("CHROMA::CreateMockFt3User Transaction failed : %s"), *ChromaUtils::STDStringToFString(error));
    });
    tx->PostAndWait([&successfully](std::string content) { 
        UE_LOG(LogTemp, Display, TEXT("CHROMA::CreateMockFt3User Transaction succeeded : %s"), *ChromaUtils::STDStringToFString(content));
        successfully = true; 
    });

    if (!successfully)
    {
        UE_LOG(LogTemp, Error, TEXT("CHROMA::CreateMockFt3User Transaction failed, return nullptr"));
        return nullptr;
    }

    std::shared_ptr<PlayerData> playerData = AuthenticateUserWithKey(
        ChromaUtils::STDStringToFString(PostchainUtil::ByteVectorToHexString(localKeypair->priv_key_)), false);

    return playerData;
}

bool UAuthService::RegisterNewPlayer(FString accountId, FString username, std::shared_ptr<PlayerData> outPlayerData, std::shared_ptr<User> outUser)
{
    bool dappWasCreated = CreateDappPlayer(accountId, username);

    if (!dappWasCreated)
    {
        UE_LOG(LogTemp, Error, TEXT("CHROMA::UAuthService::CreateDappPlayer failed"));
        return false;
    }

    TArray<FQueryObjectPair> queryObjects = { FQueryObjectPair("username", username) };

    // Fetch recently created player from blockchain
    FString playerDataStr = Query("player.find_by_username", queryObjects);

    if (playerDataStr.Len() == 0)
    {
        return nullptr;
    }

    outPlayerData = std::make_shared<PlayerData>();

    // TODO check this json parsing
    nlohmann::json json_content = nlohmann::json::parse(ChromaUtils::FStringToSTDString(playerDataStr));
    outPlayerData->m_Id = ChromaUtils::STDStringToFString(PostchainUtil::GetSafeJSONString(json_content, std::string("id")));
    outPlayerData->m_Username = ChromaUtils::STDStringToFString(PostchainUtil::GetSafeJSONString(json_content, std::string("username")));
    outPlayerData->m_Tokens = ChromaUtils::STDStringToFString(PostchainUtil::GetSafeJSONString(json_content, std::string("tokens")));
    outPlayerData->m_DateOfBirth = ChromaUtils::STDStringToFString(PostchainUtil::GetSafeJSONString(json_content, std::string("dateofbirth")));

    // TODO check if this is a valid check
    if (outPlayerData->m_Id.Len() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("CHROMA::RegisterNewPlayer outPlayerData->m_Id.Len()"));
        return false;
    }

    m_AccountId = accountId; // TODO Reinit apis after player account is created
    outUser = m_Session->user_;
    return true;
}

FString UAuthService::Query(FString queryName, TArray<FQueryObjectPair> rawQueryObjects)
{
    std::vector<QueryObject> queryObjects;
    for (size_t i = 0; i < rawQueryObjects.Num(); i++)
    {
        if (rawQueryObjects[i].m_StrContent.Len() == 0)
        {
            // This is an int type
            queryObjects.push_back(QueryObject(
                ChromaUtils::FStringToSTDString(rawQueryObjects[i].m_Name),
                AbstractValueFactory::Build(rawQueryObjects[i].m_IntContent)));
        }
        else
        {
            // This is a string type
            queryObjects.push_back(QueryObject(
                ChromaUtils::FStringToSTDString(rawQueryObjects[i].m_Name),
                AbstractValueFactory::Build(ChromaUtils::FStringToSTDString(rawQueryObjects[i].m_StrContent))));
        }   
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
            UE_LOG(LogTemp, Error, TEXT("CHROMA::UAuthService::Query failed : %s"), *ChromaUtils::STDStringToFString(error));
        }
    );

    return result;
}


bool UAuthService::CreateDappPlayer(FString accountId, FString username)
{
    std::shared_ptr<URequestService> requestService = ALoginUserDemo::GetRequestService();

    std::shared_ptr<ArrayValue> op_args;
    op_args->Add(AbstractValueFactory::Build(ChromaUtils::FStringToSTDString(username)));
    op_args->Add(AbstractValueFactory::Build(ChromaUtils::FStringToSTDString(accountId)));
    op_args->Add(AbstractValueFactory::Build(m_Session->user_->auth_descriptor_->ID()));

    bool callResult = requestService->Call({ std::make_shared<Operation>("player.new", op_args)});

    return callResult;
}
