#include "TestsMain.h"
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
#include "../chroma-cpp-pure/SSO/sso_store_local_storage.h"

#include "../chroma-cpp-pure/FT3/Core/postchain.h"

#include "../chroma-cpp-pure/src/common.h"
#include "../chroma-cpp-pure/src/postchain_transaction.h"
#include "../chroma-cpp-pure/src/GTX/gtx.h"

#include <memory>
#include <vector>

#include "CoreMinimal.h"

using namespace chromia;
using namespace chromia::postchain;
using namespace chromia::postchain::client;

void TestsMain::RunSSOTests()
{
	bool pass = false;
	SSOTest sso_test;

	pass = sso_test.SSORegisterOperationTest();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::SSORegisterOperationTest: [%d]"), pass);

	pass = sso_test.SSOAddAuthOperationTest();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::SSOAddAuthOperationTest: [%d]"), pass);
}


void TestsMain::RunRateLimitTests()
{
	bool pass = false;
	RateLimitTest rate_limit_test;

	pass = rate_limit_test.RateLimitTestRun1();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::RateLimitTestRun1: [%d]"), pass);

	pass = rate_limit_test.RateLimitTestRun2();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::RateLimitTestRun2: [%d]"), pass);

	pass = rate_limit_test.RateLimitTestRun3();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::RateLimitTestRun3: [%d]"), pass);

	pass = rate_limit_test.RateLimitTestRun4();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::RateLimitTestRun4: [%d]"), pass);

	pass = rate_limit_test.RateLimitTestRun5();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::RateLimitTestRun5: [%d]"), pass);
}

void TestsMain::RunBlockchainTests()
{
	bool pass = false;

	BlockchainTest blockchain_test;

	pass = blockchain_test.BlockchainTestRun1();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::BlockchainTestRun1: [%d]"), pass);

	pass = blockchain_test.BlockchainTestRun2();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::BlockchainTestRun2: [%d]"), pass);

	pass = blockchain_test.BlockchainTestRun3();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::BlockchainTestRun3: [%d]"), pass);

	pass = blockchain_test.BlockchainTestRun4();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::BlockchainTestRun4: [%d]"), pass);

	pass = blockchain_test.BlockchainTestRun5();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::BlockchainTestRun5: [%d]"), pass);

	pass = blockchain_test.BlockchainTestRun6();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::BlockchainTestRun6: [%d]"), pass);

	pass = blockchain_test.BlockchainTestRun7();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::BlockchainTestRun7: [%d]"), pass);

	pass = blockchain_test.BlockchainTestRun8();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::BlockchainTestRun8: [%d]"), pass);
}

void TestsMain::RunAuthDescriptorRuleTests()
{
	bool pass = false;
	AuthDescriptorRuleTest auth_descriptor_rule_test;

	pass = auth_descriptor_rule_test.AuthDescriptorRuleTestRun1();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AuthDescriptorRuleTestRun1: [%d]"), pass);

	pass = auth_descriptor_rule_test.AuthDescriptorRuleTestRun2();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AuthDescriptorRuleTestRun2: [%d]"), pass);

	pass = auth_descriptor_rule_test.AuthDescriptorRuleTestRun3();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AuthDescriptorRuleTestRun3: [%d]"), pass);

	pass = auth_descriptor_rule_test.AuthDescriptorRuleTestRun4();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AuthDescriptorRuleTestRun4: [%d]"), pass);

	pass = auth_descriptor_rule_test.AuthDescriptorRuleTestRun5();
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

	pass = auth_descriptor_rule_test.AuthDescriptorRuleTestRun11();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AuthDescriptorRuleTestRun11: [%d]"), pass);

	pass = auth_descriptor_rule_test.AuthDescriptorRuleTestRun12();
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
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AuthDescriptorRuleTestRun23: [%d]"), pass);
}

void TestsMain::RunAssetTests()
{
	bool pass = false;
	AssetTest asset_test;

	pass = asset_test.AssetTestRun1();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AccountTest1: [%d]"), pass);

	pass = asset_test.AssetTestRun2();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AccountTest2: [%d]"), pass);

	pass = asset_test.AssetTestRun3();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AccountTest3: [%d]"), pass);

	pass = asset_test.AssetTestRun4();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AccountTest4: [%d]"), pass);
}


void TestsMain::RunAssetBalanceTests()
{
	bool pass = false;
	AssetBalanceTest asset_balance_test;

	pass = asset_balance_test.AssetBalanceTestRun();
	UE_LOG(LogTemp, Warning, TEXT("RunAssetBalanceTests() [%d]"), pass);
}

void TestsMain::RunAccountTests()
{
	bool pass = false;
	AccountTest account_test;

	pass = account_test.AccountTest1();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AccountTest1: [%d]"), pass);

	pass = account_test.AccountTest2();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AccountTest2: [%d]"), pass);

	pass = account_test.AccountTest3();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AccountTest3: [%d]"), pass);

	pass = account_test.AccountTest4();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AccountTest4: [%d]"), pass);

	pass = account_test.AccountTest5();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AccountTest5: [%d]"), pass);

	pass = account_test.AccountTest6();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AccountTest6: [%d]"), pass);

	pass = account_test.AccountTest7();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AccountTest7: [%d]"), pass);

	pass = account_test.AccountTest8();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AccountTest8: [%d]"), pass);

	pass = account_test.AccountTest9();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AccountTest9: [%d]"), pass);

	pass = account_test.AccountTest10();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AccountTest10: [%d]"), pass);

	pass = account_test.AccountTest11();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AccountTest11: [%d]"), pass);

	pass = account_test.AccountTest12();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AccountTest12: [%d]"), pass);

	pass = account_test.AccountTest13();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AccountTest13: [%d]"), pass);

	pass = account_test.AccountTest14();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::AccountTest14: [%d]"), pass);
}
