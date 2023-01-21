#pragma once

#include "CoreMinimal.h"
#include "PostchainClient/common.h"
#include <vector>

using namespace chromia;

class CHROMACLIENT_API ChromaUtils
{
public:
	static std::string FStringToSTDString(const FString &fstring);

	static FString STDStringToFString(const std::string &string);

	static std::vector<byte> TArrayToSTDArray(const TArray<byte> &input);

	static TArray<byte> STDArrayToTArray(const std::vector<byte> &input);

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
