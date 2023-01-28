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

AuthService::AuthService()
{

}

AuthService::~AuthService()
{
    UE_LOG(LogTemp, Display, TEXT("CHROMA::AuthService::~AuthService()"));
}

void AuthService::Init()
{
    // TODO implement this function
    ProtocolHandler::HandleTempTx(SSO_PROTOCOL);

    SSO::SetVaultUrl(SSO_VAULT_URL);
}

std::shared_ptr<BlockchainSession> AuthService::GetSession()
{
    return m_Session;
}

std::shared_ptr<PlayerData> AuthService::AuthenticateUserWithKey(FString loginKey, bool shouldNotifySubscribers)
{
    std::shared_ptr<KeyPair> keyPair = std::make_shared<KeyPair>(ChromaUtils::FStringToSTDString(loginKey));

    if (ALoginUserDemo::GetBlockchainConnector() == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("CHROMA::AuthService::AuthenticateUserWithKey failed, BlockchainConnector is nullptr"));
        return nullptr;
    }

    m_Session = ALoginUserDemo::GetBlockchainConnector()->CreateSession(loginKey);
    std::shared_ptr<User> user = m_Session->user_;
    std::vector<std::shared_ptr<Account>> accounts;

    m_Session->GetAccountsByAuthDescriptorId(user->auth_descriptor_->ID(), 
        [&](std::vector<std::shared_ptr<Account>> _accounts) {
            accounts = _accounts;
        },
        [&](std::string error) {
            UE_LOG(LogTemp, Error, TEXT("CHROMA::AuthService::InitializeBlockchain failed : %s"),
                *ChromaUtils::STDStringToFString(error));
        }
    );

    if (accounts.size() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("CHROMA::AuthService::AuthenticateUserWithKey failed, accounts.size() == 0"));
        return nullptr;
    }

    std::shared_ptr<PlayerData> playerData = GetPlayerDataByAccountID(ChromaUtils::STDStringToFString(accounts[0]->GetID()));

    m_AccountId = ChromaUtils::STDStringToFString(accounts[0]->GetID());

    return playerData;
}

std::shared_ptr<PlayerData> AuthService::GetPlayerDataByAccountID(FString accountId)
{
    FString playerDataStr = Query("player.find_by_account_id", { FQueryObjectPair("account_id", accountId) });

    std::shared_ptr<PlayerData> playerData = std::make_shared<PlayerData>();

    if (playerDataStr.Len() == 0)
    {
        playerData->m_Id = accountId;
        playerData->m_Username = "";
    }
    else
    {
        // TODO check this json parsing
        nlohmann::json json_content = nlohmann::json::parse(ChromaUtils::FStringToSTDString(playerDataStr));
        playerData->m_Id = ChromaUtils::STDStringToFString(PostchainUtil::GetSafeJSONString(json_content, std::string("id")));
        playerData->m_Username = ChromaUtils::STDStringToFString(PostchainUtil::GetSafeJSONString(json_content, std::string("username")));
        playerData->m_Tokens = FString::FromInt(PostchainUtil::GetSafeJSONInt(json_content, std::string("tokens")));
        playerData->m_DateOfBirth = ChromaUtils::STDStringToFString(PostchainUtil::GetSafeJSONString(json_content, std::string("date_of_birth")));
    }
    
    return playerData;
}

std::shared_ptr<PlayerData> AuthService::CreateMockFt3User(std::shared_ptr<KeyPair> localKeypair)
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

bool AuthService::RegisterNewPlayer(FString accountId, FString username, std::shared_ptr<PlayerData> &outPlayerData, std::shared_ptr<User> &outUser)
{
    bool dappWasCreated = CreateDappPlayer(accountId, username);

    if (!dappWasCreated)
    {
        UE_LOG(LogTemp, Error, TEXT("CHROMA::AuthService::CreateDappPlayer failed"));
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
    outPlayerData->m_Tokens = FString::FromInt(PostchainUtil::GetSafeJSONInt(json_content, std::string("tokens")));
    outPlayerData->m_DateOfBirth = FString::FromInt(PostchainUtil::GetSafeJSONLong(json_content, std::string("tokens")));

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

bool AuthService::GivePlayerTrash(FString accountId, FString tokenName)
{
    RequestService* requestService = ALoginUserDemo::GetRequestService();

    std::shared_ptr<ArrayValue> op_args = AbstractValueFactory::EmptyArray();
    op_args->Add(AbstractValueFactory::Build(ChromaUtils::FStringToSTDString(accountId)));
    op_args->Add(AbstractValueFactory::Build(ChromaUtils::FStringToSTDString(tokenName)));
    bool callResult = requestService->Call({ std::make_shared<Operation>("assets.give_player_trash", op_args) });

    return callResult;
}

FString AuthService::GetPlayerInventory(FString accountId)
{
    TArray<FQueryObjectPair> rawQueryObjects;
    rawQueryObjects.Add(FQueryObjectPair("account_id", accountId));
    FString inventoryStr = Query("player.get_ft3_inventory", rawQueryObjects);
    return inventoryStr;
}

FString AuthService::Query(FString queryName, TArray<FQueryObjectPair> rawQueryObjects)
{
    std::vector<QueryObject> queryObjects;
    for (size_t i = 0; i < rawQueryObjects.Num(); i++)
    {
        if (rawQueryObjects[i].m_isInt)
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
    auto blockchain = ALoginUserDemo::GetBlockchainConnector()->GetBlockchain();
    blockchain->Query(
        ChromaUtils::FStringToSTDString(queryName),
        queryObjects,
        [&result](std::string content) {
            UE_LOG(LogTemp, Display, TEXT("CHROMA::AuthService::Query succeeded : %s"), *ChromaUtils::STDStringToFString(content));
            result = ChromaUtils::STDStringToFString(content);
            if (content.compare("null") == 0) 
            {
                result = "";
            }
        },
        [&result](std::string error) {
            result = "";
            UE_LOG(LogTemp, Error, TEXT("CHROMA::AuthService::Query failed : %s"), *ChromaUtils::STDStringToFString(error));
        }
    );

    return result;
}

bool AuthService::CreateDappPlayer(FString accountId, FString username)
{
    RequestService* requestService = ALoginUserDemo::GetRequestService();

    std::shared_ptr<ArrayValue> op_args = AbstractValueFactory::EmptyArray();
    op_args->Add(AbstractValueFactory::Build(ChromaUtils::FStringToSTDString(username)));
    op_args->Add(AbstractValueFactory::Build(ChromaUtils::FStringToSTDString(accountId)));
    op_args->Add(AbstractValueFactory::Build(m_Session->user_->auth_descriptor_->ID()));

    bool callResult = requestService->Call({ std::make_shared<Operation>("player.new", op_args)});

    return callResult;
}
