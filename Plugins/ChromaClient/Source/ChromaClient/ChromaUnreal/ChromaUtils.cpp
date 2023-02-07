#include "ChromaUtils.h"

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


