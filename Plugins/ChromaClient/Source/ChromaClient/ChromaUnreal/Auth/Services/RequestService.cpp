#include "RequestService.h"
#include "../../ChromaUtils.h"
#include "../../LoginUserDemo.h"
#include "FT3/Core/transaction_builder.h"
#include "FT3/Core/transaction.h"
#include "FT3/Core/Blockchain/blockchain.h"
#include "FT3/User/user.h"

RequestService::RequestService()
{

}

RequestService::~RequestService()
{
    UE_LOG(LogTemp, Display, TEXT("CHROMA::RequestService::~RequestService()"));
}

bool RequestService::Call(std::vector<std::shared_ptr<Operation>> operations)
{
    bool success = false;
    CallOperationsSync(
        operations,
        [&success]() { success = true; }, // onSuccess callback
        [] (std::string error) {  // onError callback
            UE_LOG(LogTemp, Error, TEXT("CHROMA::RequestService::CallOperationsSync failed: %s"),
                *ChromaUtils::STDStringToFString(error));
        }
    );

    if (!success)
    {
        // TODO  check error level
       // var errorData = BlockchainErrorFromException(e);
       // if (errorData.type == ErrorType.Ignorable) return;
        UE_LOG(LogTemp, Error, TEXT("CHROMA::RequestService::Call failed"));
    }

    return success;
}

void RequestService::CallOperationsSync(std::vector<std::shared_ptr<Operation>> operations, std::function<void()> onSuccess, std::function<void(std::string)> onError)
{
    std::shared_ptr<User> user = ALoginUserDemo::GetAuthService()->GetSession()->user_;
    std::shared_ptr<Blockchain> blockchain = ALoginUserDemo::GetAuthService()->GetSession()->blockchain_;

    std::shared_ptr<TransactionBuilder> tx_builder = blockchain->NewTransactionBuilder();
    for (size_t i = 0; i < operations.size(); i++)
    {
        tx_builder->Add(operations[i]);
    }

    try
    {
        std::shared_ptr<Transaction> tx = tx_builder->Build(user->auth_descriptor_->Signers(), onError);
        tx->Sign(user->key_pair_);

        tx->PostAndWait([&onSuccess](std::string content) {
            UE_LOG(LogTemp, Display, TEXT("CHROMA::CreateMockFt3User CallOperationsSync succeeded : %s"), *ChromaUtils::STDStringToFString(content));
            onSuccess();
        });
    }
    catch (std::exception& e)
    {
        UE_LOG(LogTemp, Error, TEXT("CHROMA::RequestService::CallOperationsSync failed"));
        onError(e.what());
    }
}