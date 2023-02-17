#pragma once

#include "CoreMinimal.h"
#include "PostchainClient/common.h"
#include <vector>

using namespace chromia;

#define ENV_NAME_CHROMA_BLOCKCHAIN_URL TEXT("CHROMA_BLOCKCHAIN_URL")
#define ENV_NAME_CHROMA_BRID TEXT("CHROMA_BRID")
#define ENV_NAME_CHROMA_PRIV_KEY TEXT("CHROMA_PRIV_KEY")

class ChromaUtils
{
public:
	static std::string FStringToSTDString(const FString &fstring);

	static FString STDStringToFString(const std::string &string);

	static std::vector<BYTE> TArrayToSTDArray(const TArray<BYTE> &input);

	static TArray<BYTE> STDArrayToTArray(const std::vector<BYTE> &input);

	static bool GetBlockchainConfigFromEnv(FString& blockchainURL, FString& blockchainRID, FString &privKey);

	template <typename T>
	static std::vector<T> GenericTArrayToSTDArray(const TArray<T>& input)
	{
		std::vector<T> out;
		for (T item : input)
		{
			out.push_back(item);
		}
		return out;
	}

	template <typename T>
	static TArray<T> GenericSTDArrayToTArray(const std::vector<T>& input)
	{
		TArray<T> out;
		for (T item : input)
		{
			out.Add(item);
		}
		return out;
	}
};
