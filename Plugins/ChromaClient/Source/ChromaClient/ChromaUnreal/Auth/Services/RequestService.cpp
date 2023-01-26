#include "RequestService.h"
#include "../../ChromaUtils.h"
#include "../../LoginUserDemo.h"
#include "FT3/Core/transaction_builder.h"
#include "FT3/Core/transaction.h"
#include "FT3/Core/Blockchain/blockchain.h"
#include "FT3/User/user.h"

URequestService::URequestService(const FObjectInitializer& ObjectInitializer)
{

}

void URequestService::Init(std::shared_ptr<UAuthService> authService)
{
    m_AuthService = authService;
}

bool URequestService::Call(std::vector<std::shared_ptr<Operation>> operations)
{
    /*for (int i = operations.Num() - 1; i >= 0; i--)
    {
        AssemblyCommunicationBridgeUtility.ExecuteFunction<string>("AnalyticsIndex.SendOperation", operations[i].Name);
    }*/
    bool success = false;
    CallOperationsSync(
        operations,
        [&success]() { success = true; }, // onSuccess callback
        [] (std::string error) {  // onError callback
            UE_LOG(LogTemp, Error, TEXT("CHROMA::URequestService::CallOperationsSync failed"));
        }
    );

    if (!success)
    {
        // TODO  check error level
       // var errorData = BlockchainErrorFromException(e);
       // if (errorData.type == ErrorType.Ignorable) return;
        UE_LOG(LogTemp, Error, TEXT("CHROMA::URequestService::Call failed"));
    }

    return success;
}

void URequestService::CallOperationsSync(std::vector<std::shared_ptr<Operation>> operations, std::function<void()> onSuccess, std::function<void(std::string)> onError)
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
        UE_LOG(LogTemp, Error, TEXT("CHROMA::URequestService::CallOperationsSync failed"));
        onError(e.what());
    }
}