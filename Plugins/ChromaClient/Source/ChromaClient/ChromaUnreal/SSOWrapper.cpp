
#include "SSOWrapper.h"
#include "Engine/GameEngine.h"
#include "Misc/OutputDeviceDebug.h"

#include "Utils.h"
#include "../chroma-cpp-pure/src/postchain_util.h"

#include "../chroma-cpp-pure/SSO/sso.h"
#include "../chroma-cpp-pure/SSO/protocol_handler.h"
#include "../chroma-cpp-pure/SSO/file_manager.h"
#include "../chroma-cpp-pure/FT3/Core/postchain.h"
#include "../chroma-cpp-pure/FT3/Core/Blockchain/blockchain.h"


using namespace chromia::postchain::ft3;


ASSOWrapper::ASSOWrapper(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}


void ASSOWrapper::Login()
{
	ft3::Postchain postchain("http://localhost:7740");
	std::shared_ptr<Blockchain> blockchain;
	postchain.Blockchain(ChromaUtils::FStringToSTDString(BlockchainRID),
		[&blockchain](std::shared_ptr<Blockchain> _blockchain) {
		blockchain = _blockchain;
	},
		[](std::string error) {
		UE_LOG(LogTemp, Warning, TEXT("postchain.Blockchain error: [%s]"), *ChromaUtils::STDStringToFString(error));
	});

	if (blockchain == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("blockchain == nullptr"));
		return;
	}


	SSO sso(blockchain);
	sso.InitiateLogin("http://localhost:3000/success", "http://localhost:3000/error");

	while (sso.store_->GetTmpTx().size() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Wait for temp file update"));
		PostchainUtil::SleepForMillis(3000);
		sso.store_->Load();
	}

	std::string payload = sso.store_->GetTmpTx();

	sso.FinalizeLogin(payload, 
	[this](SSO::AccUserPair user_pair) {
		UE_LOG(LogTemp, Warning, TEXT("Authentication success for account: [%s]"), *ChromaUtils::STDStringToFString(user_pair.account->id_));
		PrintLogOnScreen(FString("Authentication success for user: ") + ChromaUtils::STDStringToFString(user_pair.account->id_));
	},
	[](std::string content) {
		UE_LOG(LogTemp, Warning, TEXT("Authentication failed with error: [%s]"), *ChromaUtils::STDStringToFString(content));
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


void ASSOWrapper::PrintLogOnScreen(FString message)
{
	if (MainWidget == nullptr) return;

	const FString command = FString::Printf(TEXT("PrintLog %s"), *message);
	FOutputDeviceDebug debug;
	MainWidget->CallFunctionByNameWithArguments(*command, debug, this, true);
}
