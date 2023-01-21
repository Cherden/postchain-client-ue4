
#include "SSOWrapper.h"

#include "Engine/GameEngine.h"
#include "Misc/OutputDeviceDebug.h"
#include "Async/Async.h"
#include "Utils.h"

#include "PostchainClient/postchain_util.h"
#include "SSO/sso.h"
#include "SSO/file_manager.h"
#include "FT3/Core/postchain.h"
#include "FT3/Core/Blockchain/blockchain.h"


using namespace chromia::postchain::ft3;


ASSOWrapper::ASSOWrapper(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

namespace
{
	bool authOk = true;
	FString authMessage = "";
}

void ASSOWrapper::Login()
{
	if (this->LoginInProgress == true)
	{
		PrintLogOnScreen(FString("One authentication request is already active, waiting..."));
		return;
	}

	this->LoginInProgress = true;
	authOk = true;
	authMessage = "";

	// Perform authentication procedure async
	AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [&]()
	{

		//ft3::Postchain postchain("http://localhost:7740");
		ft3::Postchain postchain(ChromaUtils::FStringToSTDString(BaseURL));
		std::shared_ptr<Blockchain> blockchain;
		postchain.Blockchain(ChromaUtils::FStringToSTDString(BlockchainRID),
			[&blockchain](std::shared_ptr<Blockchain> _blockchain) {
				blockchain = _blockchain;
			},
			[](std::string error) {
				authOk = false;
				authMessage = FString::Printf(TEXT("postchain.Blockchain error: %s"), *ChromaUtils::STDStringToFString(error));
			});

		if (authOk && blockchain == nullptr)
		{
			authOk = false;
			authMessage = "Failed to initialize blockchain connection";
		}

		if (authOk)
		{
			SSO sso(blockchain);
			sso.InitiateLogin("http://localhost:3000/success", "http://localhost:3000/error");

			// Wait for sso store to receive the updated value from authentication
			while (sso.store_->GetTmpTx().size() == 0)
			{
				PostchainUtil::SleepForMillis(3000);
				sso.store_->Load();
			}

			std::string payload = sso.store_->GetTmpTx();

			// Move back to the gamethread.
			AsyncTask(ENamedThreads::GameThread, [&]()
			{
				sso.FinalizeLogin(payload,
				[this](SSO::AccUserPair user_pair) {
					UE_LOG(LogTemp, Warning, TEXT("Authentication success for account: [%s]"), *ChromaUtils::STDStringToFString(user_pair.account->id_));
					PrintLogOnScreen(FString("Authentication success for user: ") + ChromaUtils::STDStringToFString(user_pair.account->id_));
				},
				[this](std::string error) {
					FString message = FString::Printf(TEXT("Authentication failed with error: %s"), *ChromaUtils::STDStringToFString(error));
					PrintLogOnScreen(message);
				});

				this->LoginInProgress = false;
			});
		}
		else 
		{
			// Move back to the gamethread.
			AsyncTask(ENamedThreads::GameThread, [&]()
			{
				PrintLogOnScreen(authMessage);
				this->LoginInProgress = false;
			});
		}
	
	});
}


void ASSOWrapper::SetMainWidget(UObject* mw)
{
	this->MainWidget = mw;
}


void ASSOWrapper::Setup(FString blockchainRID, FString baseURL)
{
	this->BlockchainRID = blockchainRID;
	this->BaseURL = baseURL;
}

bool ASSOWrapper::IsLoginInProgress()
{
	return this->LoginInProgress;
}

void ASSOWrapper::PrintLogOnScreen(FString message)
{
	if (MainWidget == nullptr) return;

	const FString command = FString::Printf(TEXT("PrintLog %s"), *message);
	FOutputDeviceDebug debug;
	MainWidget->CallFunctionByNameWithArguments(*command, debug, this, true);
}
