#pragma once

#include "CoreMinimal.h"
#include "../chroma-cpp-pure/src/common.h"
#include <vector>

using namespace chromia;

class CHROMACLIENT_API ChromaUtils
{
public:
	static std::string FStringToSTDString(const FString &fstring);

	static FString STDStringToFString(const std::string &string);

	static std::vector<byte> TArrayToSTDArray(const TArray<byte> &input);

	static TArray<byte> STDArrayToTArray(const std::vector<byte> &input);
};
