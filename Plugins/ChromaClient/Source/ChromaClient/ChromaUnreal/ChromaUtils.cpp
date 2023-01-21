#include "ChromaUtils.h"

std::string ChromaUtils::FStringToSTDString(const FString &fstring)
{
	return std::string(TCHAR_TO_UTF8(*fstring));
}


FString ChromaUtils::STDStringToFString(const std::string &string)
{
	return FString(UTF8_TO_TCHAR(string.c_str()));
}


std::vector<byte> ChromaUtils::TArrayToSTDArray(const TArray<byte> &input)
{
	std::vector<byte> out;
	for (byte item : input)
	{
		out.push_back(item);
	}
	return out;
}


TArray<byte> ChromaUtils::STDArrayToTArray(const std::vector<byte> &input)
{
	TArray<byte> out;
	for (byte item : input)
	{
		out.Add(item);
	}
	return out;
}


