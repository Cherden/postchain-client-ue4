//#pragma once
//
//#include "CoreMinimal.h"
//#include "Kismet/BlueprintFunctionLibrary.h"
//#include "../Chroma/common.h"
////#include "../SECP256K/include/secp256k1.h"
//
//using namespace chromia;
//
///**
//* PostchainUtil Singleton
//*/
//
//class CHROMACLIENT_API PostchainUtil {
//
//public:
//
//	static PostchainUtil* GetInstance();
//	
//	PostchainUtil();
//
//	///**
//	//  * Creates a SECP256K key pair (which usually represents one user)
//	//  * @param Out - Reference to TMap {"pubKey": Buffer, "privKey": Buffer}
//	//  * @return true if the execution was successfull, false otherwise
//	//  */
//	//bool GenerateKeyPair(TMap<FString, TArray<byte>> &Out);
//
//	///**
//	//  * Converts hex string to Buffer
//	//  * @param key: string
//	//  * @returns {Buffer}
//	//  */
//	//static TArray<byte> HexStringToBuffer(FString hex) 
//	//{
//	//	//!!! TO-DO TEST THAT RETURNED VALUE IS IDENTICAL TO C# !!!
//	//	TArray<byte> buffer;
//	//	for (int i = 0; i < hex.Len(); i++) {
//	//		if (i % 2 != 0) continue;
//
//	//		FString substr = hex.Mid(i, 2);
//	//		buffer.Add((uint8) FCString::Atoi(*hex));
//	//	}
//	//	return buffer;
//	//}
//
//private:
//	static PostchainUtil* Instance;
//
//	secp256k1_context *Context;
//	
//	/* Init context secp256k1_context. This should be called only once from the constructor */
//	void InitContext();
//
//	/**
//	  * Creates a SECP256K private key
//	  * @param Out - Reference to the key wich will be created
//	  * @return true if the execution was successfull, false otherwise
//	  */
//	bool GeneratePrivateKey(TArray<byte>& PrivateKey);
//
//	/**
//	  * Creates a SECP256K public key
//	  * @param Out - Reference to the key wich will be created
//	  * @return true if the execution was successfull, false otherwise
//	  */
//	bool GenerateCompresedPublicKey(TArray<byte> PrivateKey, TArray<byte>& PublicKey);
//
//	/** Get string representation of some binary private or public key */
//	FString KeyToString(const TArray<byte> &key);
//};
