#pragma once

#include "CoreMinimal.h"
#include "PostchainClient/common.h"
#include <vector>

using namespace chromia;

#define BLOCKCHAIN_CONFIG_JSON_FILE "blockchain-config.json"

class ChromaUtils
{
public:
	static std::string FStringToSTDString(const FString &fstring);

	static FString STDStringToFString(const std::string &string);

	static std::vector<BYTE> TArrayToSTDArray(const TArray<BYTE> &input);

	static TArray<BYTE> STDArrayToTArray(const std::vector<BYTE> &input);

	static bool GetBlockchainConfigFromFile(FString& url, FString& brid);

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
