#include "ChromaUtils.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "SSO/file_manager.h"

using namespace chromia::postchain::ft3;

std::string ChromaUtils::FStringToSTDString(const FString &fstring)
{
	return std::string(TCHAR_TO_UTF8(*fstring));
}


FString ChromaUtils::STDStringToFString(const std::string &string)
{
	return FString(UTF8_TO_TCHAR(string.c_str()));
}


std::vector<BYTE> ChromaUtils::TArrayToSTDArray(const TArray<BYTE> &input)
{
	std::vector<BYTE> out;
	for (BYTE item : input)
	{
		out.push_back(item);
	}
	return out;
}


TArray<BYTE> ChromaUtils::STDArrayToTArray(const std::vector<BYTE> &input)
{
	TArray<BYTE> out;
	for (BYTE item : input)
	{
		out.Add(item);
	}
	return out;
}

bool ChromaUtils::GetBlockchainConfigFromEnv(FString& blockchainURL, FString& blockchainRID, FString& privKey)
{
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::GetBlockchainConfigFromEnv()"));

	blockchainURL = FPlatformMisc::GetEnvironmentVariable(ENV_NAME_CHROMA_BLOCKCHAIN_URL);
	blockchainRID = FPlatformMisc::GetEnvironmentVariable(ENV_NAME_CHROMA_BRID);
	privKey = FPlatformMisc::GetEnvironmentVariable(ENV_NAME_CHROMA_PRIV_KEY);

	UE_LOG(LogTemp, Display, TEXT("Environment config value: [%s] = [%s]"), ENV_NAME_CHROMA_BLOCKCHAIN_URL, *blockchainURL);
	UE_LOG(LogTemp, Display, TEXT("Environment config value: [%s] = [%s]"), ENV_NAME_CHROMA_BRID, *blockchainRID);
	UE_LOG(LogTemp, Display, TEXT("Environment config value: [%s] = [%s]"), ENV_NAME_CHROMA_PRIV_KEY, *privKey);

	if (blockchainURL.Len() == 0) 
	{
		UE_LOG(LogTemp, Error, TEXT("Could not read value from [%s]"), ENV_NAME_CHROMA_BLOCKCHAIN_URL);
		return false;
	}

	if (blockchainRID.Len() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not read value from [%s]"), ENV_NAME_CHROMA_BRID);
		return false;
	}
	
	if (privKey.Len() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not read value from [%s]"), ENV_NAME_CHROMA_PRIV_KEY);
		return false;
	}

	return true;
}


