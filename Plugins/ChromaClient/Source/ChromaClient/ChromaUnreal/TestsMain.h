#pragma once

class TestsMain {
public:
	TestsMain();

	void RunSSOTests();

	void RunRateLimitTests();
	
	void RunBlockchainTests();
	
	void RunAuthDescriptorRuleTests();
	
	void RunAssetTests();
	
	void RunAssetBalanceTests();

	void RunAccountTests();
};
