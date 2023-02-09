#include "ChromaUtils.h"
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

bool ChromaUtils::GetBlockchainConfigFromFile(FString& blockchainURL, FString& blockchainRID, FString& privKey)
{
	FString configJsonPath = FPaths::Combine(FPaths::ProjectConfigDir(), BLOCKCHAIN_CONFIG_JSON_FILE);
	std::string dataStr = "";

	if (!ChromaFileManager::LoadFromAbsFile(FStringToSTDString(configJsonPath), dataStr))
	{
		UE_LOG(LogTemp, Warning, TEXT("CHROMA::ChromaUtils::GetBlockchainConfigFromFile failed to read from %s"), *configJsonPath);
		return false;
	}

	if (dataStr.size() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("CHROMA::ChromaUtils::GetBlockchainConfigFromFile dataStr.size() == 0"));
		return false;
	}

	nlohmann::json json_obj = nlohmann::json::parse(dataStr);
	
	if (!json_obj.contains("blockchainUrl"))
	{
		UE_LOG(LogTemp, Warning, TEXT("CHROMA::ChromaUtils::GetBlockchainConfigFromFile !json_obj.contains[blockchainUrl]"));
		return false;
	}

	if (!json_obj.contains("brid"))
	{
		UE_LOG(LogTemp, Warning, TEXT("CHROMA::ChromaUtils::GetBlockchainConfigFromFile !json_obj.contains[brid]"));
		return false;
	}

	if (!json_obj.contains("privKey"))
	{
		UE_LOG(LogTemp, Warning, TEXT("CHROMA::ChromaUtils::GetBlockchainConfigFromFile !json_obj.contains[privKey]"));
		return false;
	}

	blockchainURL = STDStringToFString(json_obj["blockchainUrl"]);
	blockchainRID = STDStringToFString(json_obj["brid"]);
	privKey = STDStringToFString(json_obj["privKey"]);

	return true;
}


