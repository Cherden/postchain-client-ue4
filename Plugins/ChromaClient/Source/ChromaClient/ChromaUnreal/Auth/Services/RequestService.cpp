#include "RequestService.h"

ARequestService::ARequestService(const FObjectInitializer& ObjectInitializer)
{

}

void ARequestService::Init(std::shared_ptr<AAuthService> authService)
{
    m_AuthService = authService;
}

//void ARequestService::Call(TArray<Operation> operations)
//{
    // TODO implement this
    /*for (int i = operations.Num() - 1; i >= 0; i--)
    {
        AssemblyCommunicationBridgeUtility.ExecuteFunction<string>("AnalyticsIndex.SendOperation", operations[i].Name);
    }*/

   /* try
    {
        var callCompleted = new UniTaskCompletionSource();
        StartCoroutine(CallCoroutine(
            operations,
            () = > callCompleted.TrySetResult(),
            (err) = > callCompleted.TrySetException(new Exception(err))
        ));

        await callCompleted.Task;
    }
    catch (Exception e)
    {
        var errorData = BlockchainErrorFromException(e);

        if (errorData.type == ErrorType.Ignorable) return;

        throw new RequestErrorException(errorData);
    }*/
//}

//void ARequestService::CallOperationsSync(TArray<Operation> operations, std::function<void(std::string)> on_error, std::function<void(std::string)> onError)
//{
   /* std::shared_ptr<User> user = m_AuthService->GetSession()->user_;

    var transactionBuilder = _authService.session.Blockchain.TransactionBuilder();
    foreach(var op in operations) transactionBuilder.Add(op);

    yield return transactionBuilder.Build(user.AuthDescriptor.Signers.ToArray(), onError)
        .Sign(user.KeyPair)
        .PostAndWait(onSuccess);*/
//}