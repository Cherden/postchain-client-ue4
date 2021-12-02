
#include "BlockchainClient.h"
#include "Engine/GameEngine.h"
#include "Misc/OutputDeviceDebug.h"

#include "Utils.h"
#include "../chroma-cpp-pure/src/postchain_util.h"

#include "../chroma-cpp-pure/tests/FT3/asset_test.h"
#include "../chroma-cpp-pure/tests/FT3/account_test.h"
#include "../chroma-cpp-pure/tests/FT3/asset_balance_test.h"
#include "../chroma-cpp-pure/tests/FT3/auth_descriptor_rule_test.h"
#include "../chroma-cpp-pure/tests/FT3/blockchain_test.h"
#include "../chroma-cpp-pure/tests/FT3/rate_limit_test.h"
#include "../chroma-cpp-pure/tests/SSO/sso_test.h"

#include "../chroma-cpp-pure/SSO/sso.h"
#include "../chroma-cpp-pure/SSO/protocol_handler.h"
#include "../chroma-cpp-pure/SSO/file_manager.h"
#include "../chroma-cpp-pure/FT3/Core/postchain.h"

ABlockchainClient::ABlockchainClient(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bool pass = false;

	/*SSOTest sso_test;

	pass = sso_test.SSORegisterOperationTest();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::SSORegisterOperationTest: [%d]"), pass);

	pass = sso_test.SSOAddAuthOperationTest();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::SSOAddAuthOperationTest: [%d]"), pass);*/

	//-----------------------TEST SSO--------------------------
	/*ft3::Postchain postchain("http://localhost:7740");
	std::shared_ptr<Blockchain> blockchain;
	postchain.Blockchain("849AD8C9AC720A21962187D0BDA6168DA274E1D17D39AAD513559171FDDC6914",
		[&blockchain] (std::shared_ptr<Blockchain> _blockchain) {
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
	UE_LOG(LogTemp, Warning, TEXT("Wait for temp file update: [%s]"), *ChromaUtils::STDStringToFString(payload));

	sso.FinalizeLogin(payload, [](SSO::AccUserPair user_pair) {
		UE_LOG(LogTemp, Warning, TEXT("Authentication success for account: [%s]"), *ChromaUtils::STDStringToFString(user_pair.account->id_));
	}, 
	[](std::string content) {
		UE_LOG(LogTemp, Warning, TEXT("Authentication failed with error: [%s]"), *ChromaUtils::STDStringToFString(content));
	});

	return;*/
	/*

	
	//ProtocolHandler::Register("ChromiaProtocol");



	/*_sso.InitiateLogin(_successUrl, _cancelUrl);

	while (_sso.Store.TmpTx == null)
	{
		yield return new WaitForSeconds(3);
		_sso.Store.Load();
	}

	var payload = _sso.Store.TmpTx;
	payload = payload.Split("?"[0])[1];
	string raw = payload.Split("="[0])[1];

	yield return _sso.FinalizeLogin(raw, PanelManager.AddOptionToPanel, DefaultErrorHandler);*/

	//---------------------------------------------------------

	//RateLimitTest rate_limit_test;

	/*pass = rate_limit_test.RateLimitTestRun1();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::RateLimitTestRun1: [%d]"), pass);

	pass = rate_limit_test.RateLimitTestRun2();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::RateLimitTestRun2: [%d]"), pass);

	pass = rate_limit_test.RateLimitTestRun3();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::RateLimitTestRun3: [%d]"), pass);

	pass = rate_limit_test.RateLimitTestRun4();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::RateLimitTestRun4: [%d]"), pass);

	pass = rate_limit_test.RateLimitTestRun5();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::RateLimitTestRun5: [%d]"), pass);
	*/

	//BlockchainTest blockchain_test;

	/*pass = blockchain_test.BlockchainTestRun1();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::BlockchainTestRun1: [%d]"), pass);

	pass = blockchain_test.BlockchainTestRun2();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::BlockchainTestRun2: [%d]"), pass);

	pass = blockchain_test.BlockchainTestRun3();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::BlockchainTestRun3: [%d]"), pass);*/

	/*pass = blockchain_test.BlockchainTestRun4();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::BlockchainTestRun4: [%d]"), pass);*/

	//pass = blockchain_test.BlockchainTestRun5();
	//UE_LOG(LogTemp, Warning, TEXT("CHROMA::BlockchainTestRun5: [%d]"), pass);

	//pass = blockchain_test.BlockchainTestRun6();
	//UE_LOG(LogTemp, Warning, TEXT("CHROMA::BlockchainTestRun6: [%d]"), pass);

	/*pass = blockchain_test.BlockchainTestRun7();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::BlockchainTestRun7: [%d]"), pass);

	pass = blockchain_test.BlockchainTestRun8();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::BlockchainTestRun8: [%d]"), pass);*/

	/*AuthDescriptorRuleTest auth_descriptor_rule_test;

	pass = auth_descriptor_rule_test.AuthDescriptorRuleTestRun1();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AuthDescriptorRuleTestRun1: [%d]"), pass);

	pass = auth_descriptor_rule_test.AuthDescriptorRuleTestRun2();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AuthDescriptorRuleTestRun2: [%d]"), pass);*/
	
	/*pass = auth_descriptor_rule_test.AuthDescriptorRuleTestRun3();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AuthDescriptorRuleTestRun3: [%d]"), pass);
	
	pass = auth_descriptor_rule_test.AuthDescriptorRuleTestRun4();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AuthDescriptorRuleTestRun4: [%d]"), pass);*/

	/*pass = auth_descriptor_rule_test.AuthDescriptorRuleTestRun5();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AuthDescriptorRuleTestRun5: [%d]"), pass);
	
	pass = auth_descriptor_rule_test.AuthDescriptorRuleTestRun6();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AuthDescriptorRuleTestRun6: [%d]"), pass);

	pass = auth_descriptor_rule_test.AuthDescriptorRuleTestRun7();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AuthDescriptorRuleTestRun7: [%d]"), pass);

	pass = auth_descriptor_rule_test.AuthDescriptorRuleTestRun8();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AuthDescriptorRuleTestRun8: [%d]"), pass);

	pass = auth_descriptor_rule_test.AuthDescriptorRuleTestRun9();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AuthDescriptorRuleTestRun9: [%d]"), pass);

	pass = auth_descriptor_rule_test.AuthDescriptorRuleTestRun10();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AuthDescriptorRuleTestRun10: [%d]"), pass);
	*/

	/*pass = auth_descriptor_rule_test.AuthDescriptorRuleTestRun11();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AuthDescriptorRuleTestRun11: [%d]"), pass);*/

	/*pass = auth_descriptor_rule_test.AuthDescriptorRuleTestRun12();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AuthDescriptorRuleTestRun12: [%d]"), pass);

	pass = auth_descriptor_rule_test.AuthDescriptorRuleTestRun13();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AuthDescriptorRuleTestRun13: [%d]"), pass);

	pass = auth_descriptor_rule_test.AuthDescriptorRuleTestRun14();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AuthDescriptorRuleTestRun14: [%d]"), pass);

	pass = auth_descriptor_rule_test.AuthDescriptorRuleTestRun15();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AuthDescriptorRuleTestRun15: [%d]"), pass);

	pass = auth_descriptor_rule_test.AuthDescriptorRuleTestRun16();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AuthDescriptorRuleTestRun16: [%d]"), pass);

	pass = auth_descriptor_rule_test.AuthDescriptorRuleTestRun17();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AuthDescriptorRuleTestRun17: [%d]"), pass);

	pass = auth_descriptor_rule_test.AuthDescriptorRuleTestRun18();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AuthDescriptorRuleTestRun18: [%d]"), pass);

	pass = auth_descriptor_rule_test.AuthDescriptorRuleTestRun19();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AuthDescriptorRuleTestRun19: [%d]"), pass);

	pass = auth_descriptor_rule_test.AuthDescriptorRuleTestRun20();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AuthDescriptorRuleTestRun20: [%d]"), pass);

	pass = auth_descriptor_rule_test.AuthDescriptorRuleTestRun21();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AuthDescriptorRuleTestRun21: [%d]"), pass);

	pass = auth_descriptor_rule_test.AuthDescriptorRuleTestRun22();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AuthDescriptorRuleTestRun22: [%d]"), pass);

	pass = auth_descriptor_rule_test.AuthDescriptorRuleTestRun23();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AuthDescriptorRuleTestRun23: [%d]"), pass);*/

	/*AssetTest asset_test;

	pass = asset_test.AssetTestRun1();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AccountTest1: [%d]"), pass);
	
	pass = asset_test.AssetTestRun2();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AccountTest2: [%d]"), pass);

	pass = asset_test.AssetTestRun3();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AccountTest3: [%d]"), pass);
	
	pass = asset_test.AssetTestRun4();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AccountTest4: [%d]"), pass);*/

	/*AssetBalanceTest asset_balance_test;
	bool pass = asset_balance_test.AssetBalanceTestRun();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AssetBalanceTestRun() [%d]"), pass);*/
	
	//AccountTest account_test;

	/*pass = account_test.AccountTest1();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AccountTest1: [%d]"), pass);*/

	/*pass = account_test.AccountTest2();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AccountTest2: [%d]"), pass);*/

	/*pass = account_test.AccountTest3();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AccountTest3: [%d]"), pass);*/

	/*pass = account_test.AccountTest4();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AccountTest4: [%d]"), pass);*/

	/*pass = account_test.AccountTest5();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AccountTest5: [%d]"), pass);*/

	/*pass = account_test.AccountTest6();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AccountTest6: [%d]"), pass);*/

	/*pass = account_test.AccountTest7();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AccountTest7: [%d]"), pass);*/

	/*pass = account_test.AccountTest8();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AccountTest8: [%d]"), pass);*/

	/*pass = account_test.AccountTest9();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AccountTest9: [%d]"), pass);*/

	/*pass = account_test.AccountTest10();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AccountTest10: [%d]"), pass);*/

	/*pass = account_test.AccountTest11();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AccountTest11: [%d]"), pass);*/

	/*pass = account_test.AccountTest12();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AccountTest12: [%d]"), pass);*/

	/*pass = account_test.AccountTest13();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AccountTest13: [%d]"), pass);*/

	//pass = account_test.AccountTest14(); // TO-DO
	//UE_LOG(LogTemp, Warning, TEXT("CHROMA::AccountTest14: [%d]"), pass);

	//this->BlockchainRID = "1711E1936B67292DB3D6E72DFBB8969DB12F199E2E6D75F9138011A3343FAB95";
	//this->BaseURL = "https://rellide-staging.chromia.dev/node/16283/";

	std::vector<unsigned char> private_key;
	std::vector<unsigned char> public_key;
	if (!PostchainUtil::GenerateKeyPair(private_key, public_key))
	{
		UE_LOG(LogTemp, Error, TEXT("CHROMA::UBlockchainClient::GenerateKeyPair failed"));
	}
	else 
	{
		PrivateKey = ChromaUtils::STDArrayToTArray(private_key);
		PublicKey = ChromaUtils::STDArrayToTArray(public_key);
	}

	
	//RegisterUser("abcdef");
}


void ABlockchainClient::SetMainWidget(UObject* mw)
{
	this->MainWidget = mw;
}


void ABlockchainClient::Setup(FString blockchainRID, FString baseURL)
{
	this->BlockchainRID = blockchainRID;
	this->BaseURL = baseURL;
}


TSharedPtr<PostchainTransaction> ABlockchainClient::NewTransaction(TArray<TArray<byte>> signers)
{
	std::shared_ptr<Gtx> gtx = std::make_shared<Gtx>(ChromaUtils::FStringToSTDString(this->BlockchainRID));

	for(TArray<byte> &signer : signers)
	{
		std::vector<byte> arr = ChromaUtils::TArrayToSTDArray(signer);
		gtx->AddSignerToGtx(arr);
	}

	TSharedPtr<PostchainTransaction> transaction = MakeShared<PostchainTransaction>(
		gtx, 
		ChromaUtils::FStringToSTDString(this->BaseURL), 
		ChromaUtils::FStringToSTDString(this->BlockchainRID),
		[](std::string error) {});
	return transaction;
}


void ABlockchainClient::RegisterUser(FString username)
{
	if (!KeyPairIsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("CHROMA::RegisterUser fail. Missing KeyPair"));
		return;
	}
	TSharedPtr<PostchainTransaction> transaction = NewTransaction({PublicKey});

	std::shared_ptr<gtv::ArrayValue> operation_values = AbstractValueFactory::EmptyArray();
	operation_values->Add(AbstractValueFactory::Build(ChromaUtils::FStringToSTDString(username)));
	transaction->AddOperation(std::string("register_user"), operation_values);

	///*
	//	The blockchain hashes the operations to generate a transaction id (txid) which has to be unique.
	//	In order to prevent this colision, a "nop" operation can be attached which will be filtered by
	//	the blockchain.
	//*/
	std::shared_ptr<gtv::ArrayValue> nop_operation_values = AbstractValueFactory::EmptyArray();
	static int nonce = PostchainUtil::RandomIntInRange(0, 100000);
	UE_LOG(LogTemp, Display, TEXT("CHROMA transaction nop nonce: %d"), nonce);
	nop_operation_values->Add(AbstractValueFactory::Build(std::to_string(nonce)));
	transaction->AddOperation(std::string("nop"), nop_operation_values);

	transaction->Sign(ChromaUtils::TArrayToSTDArray(PrivateKey), ChromaUtils::TArrayToSTDArray(PublicKey));

	TxRID = ChromaUtils::STDStringToFString(transaction->GetTxRID());

	// -------------------------- POST -------------------------
	
	//var payload = String.Format(@"{{""tx"": ""{0}""}}", Serialize());

	std::string stdSerialized = transaction->Serialize();
	FString serialized = ChromaUtils::STDStringToFString(stdSerialized);

	FString payload = FString::Printf(TEXT("{\"tx\": \"%s\"}"), *serialized);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &ABlockchainClient::OnTransactionResponseReceived);

	FString url = FString::Printf(TEXT("%s/tx/%s"), *(this->BaseURL), *(this->BlockchainRID));
	UE_LOG(LogTemp, Display, TEXT("CHROMA::URL : [%s]"), *url);
	UE_LOG(LogTemp, Display, TEXT("CHROMA::Payload Length : [%d]"), payload.Len());
	UE_LOG(LogTemp, Display, TEXT("CHROMA::Payload : [%s]"), *payload);

	Request->SetURL(*url);

	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	//Request->SetHeader(TEXT("Accepts"), TEXT("application/json"));

	Request->SetContentAsString(*payload);
	//Request->SetHeader("Content-Type", "application/json");

	//Send the request
	Request->ProcessRequest();

	UE_LOG(LogTemp, Display, TEXT("CHROMA::ProcessRequest();"));

	WaitForBlockchainConfirmation();

	return;
}


void ABlockchainClient::CheckUser(FString username)
{
	//--------------------------------------------------------------------------
	// Prepare query object
	std::vector<QueryObject> queryObjects;
	queryObjects.push_back(QueryObject("name", AbstractValueFactory::Build(ChromaUtils::FStringToSTDString(username))));
	//---------------------------------------------------------------------------
	
	//---------------------------------------------------------------------------
	// Send POST request
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();

	// Reqister callback
	Request->OnProcessRequestComplete().BindUObject(this, &ABlockchainClient::OnQueryResponseReceived);

	FString url = FString::Printf(TEXT("%s/query/%s"), *(this->BaseURL), *(this->BlockchainRID));
	Request->SetURL(*url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	// Add request content as json
	std::string json = PostchainUtil::QueryToJSONString("check_user", queryObjects);
	FString payload = ChromaUtils::STDStringToFString(json);
	Request->SetContentAsString(*payload);

	// Send the request
	Request->ProcessRequest();
	//---------------------------------------------------------------------------
}


void ABlockchainClient::WaitForBlockchainConfirmation()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();

	// Reqister callback
	Request->OnProcessRequestComplete().BindUObject(this, &ABlockchainClient::OnBlockchainConfirmationReceived);

	FString url = FString::Printf(TEXT("%s/tx/%s/%s/status"), *(this->BaseURL), *(this->BlockchainRID), *(this->TxRID));

	Request->SetURL(*url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");

	//Send the request
	Request->ProcessRequest();
}


void ABlockchainClient::InitializeBRIDFromChainID()
{
	/*std::function<void(int, std::string)> callback = [](int error, std::string content) {
		UE_LOG(LogTemp, Error, TEXT("CHROMA::onSuccess lambda called: %d  %s"), error, *(ChromaUtils::STDStringToFString(content)));
	};

	FString url = FString::Printf(TEXT("%s/brid/iid_%d"), *(this->BaseURL), this->ChainID);
	chromia::postchain::http::HttpRequest request(ChromaUtils::FStringToSTDString(url), callback);
	request.Get();*/

	// HTTPS
	//FString url = FString::Printf(TEXT("%s/brid/iid_%d"), *(this->BaseURL), this->ChainID);
	
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();

	// Reqister callback
	Request->OnProcessRequestComplete().BindUObject(this, &ABlockchainClient::OnBRIDResponseReceived);

	FString url = FString::Printf(TEXT("%s/brid/iid_%d"), *(this->BaseURL), this->ChainID);
	Request->SetURL(*url);

	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");

	//Send the request
	Request->ProcessRequest();
}


void ABlockchainClient::OnBRIDResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Display, TEXT("CHROMA::OnBRIDResponseReceived. Valid: %d"), bWasSuccessful);

	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Display, TEXT("CHROMA::OnBRIDResponseReceived %s"), *(Response->GetContentAsString()));
		this->BlockchainRID = Response->GetContentAsString();
	}
}


void ABlockchainClient::OnTransactionResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Display, TEXT("CHROMA::OnTransactionResponseReceived. Valid: %d"), bWasSuccessful);

	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Display, TEXT("CHROMA::OnTransactionResponseReceived %s"), *(Response->GetContentAsString()));
		PrintLogOnScreen("Transaction sent");
	}
}

void ABlockchainClient::OnBlockchainConfirmationReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Display, TEXT("CHROMA::OnBlockchainConfirmationReceived. Valid: %d"), bWasSuccessful);

	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Display, TEXT("CHROMA::OnBlockchainConfirmationReceived %s"), *(Response->GetContentAsString()));

		FString content = Response->GetContentAsString();
		if (content.StartsWith("{")) // Check for json content
		{
			nlohmann::json jsonContent = nlohmann::json::parse(ChromaUtils::FStringToSTDString(content));

			if (jsonContent.contains("status")) {
				std::string stdStatus = jsonContent["status"];
				FString status = ChromaUtils::STDStringToFString(stdStatus);
				if (status.Equals("waiting"))
				{
					WaitForBlockchainConfirmation();
				}
				else if (status.Equals("confirmed"))
				{
					PrintLogOnScreen(TEXT("User registered successfully!"));
				}
				else
				{
					PrintLogOnScreen(FString::Printf(TEXT("Failed to register user. status: %s"), *status));
				}
			}
			else {
				PrintLogOnScreen(TEXT("Invalid blockchain response"));
			}
		}
		else 
		{
			PrintLogOnScreen(TEXT("Invalid response blockchain response, check connection."));
		}
	}
}


void ABlockchainClient::OnQueryResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		FString response = Response->GetContentAsString();

		if (response.Equals("1"))
		{
			PrintLogOnScreen(TEXT("User is registered"));
		}
		else 
		{
			PrintLogOnScreen(TEXT("User is not registered"));
		}
	}
	else 
	{
		PrintLogOnScreen(TEXT("Invalid query response"));
	}
}


void ABlockchainClient::PrintLogOnScreen(FString message)
{
	if (MainWidget == nullptr) return;

	const FString command = FString::Printf(TEXT("PrintLog %s"), *message);
	FOutputDeviceDebug debug;
	MainWidget->CallFunctionByNameWithArguments(*command, debug, this, true);
}


bool ABlockchainClient::KeyPairIsValid()
{
	if (!(PrivateKey.Num() == 32)) return false;
	if (!(PublicKey.Num() == 33)) return false;
	return true;
}
