//#include "PostchainUtil.h"
//
//
//PostchainUtil* PostchainUtil::Instance = nullptr;
//
//
//PostchainUtil::PostchainUtil()
//{
//	InitContext();
//}
//
//
//PostchainUtil* PostchainUtil::GetInstance()
//{
//	if (PostchainUtil::Instance == nullptr)
//	{
//		PostchainUtil::Instance = new PostchainUtil();
//	}
//	return PostchainUtil::Instance;
//}
//
//
//void PostchainUtil::InitContext()
//{
//	this->Context = secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY);
//}
//
//
//bool PostchainUtil::GeneratePrivateKey(TArray<byte>& PrivateKey)
//{
//	// Init private key
//	for (int i = 0; i < 32; i++) 
//	{
//		PrivateKey.Add(33 + i); // !!! IMPORTANT TO-DO randomize !!!
//	}
//
//	//Check private key
//	int result = secp256k1_ec_seckey_verify(this->Context, PrivateKey.GetData());
//	if (result == 0) 
//	{
//		UE_LOG(LogTemp, Error, TEXT("CHROMA::secp256k1_ec_seckey_verify failed"));
//		return false;
//	}
//
//	return true;
//}
//
//
//bool PostchainUtil::GenerateCompresedPublicKey(TArray<byte> PrivateKey, TArray<byte>& PublicKey)
//{
//	// Generate secp256k1 raw public key from private key
//	secp256k1_pubkey rawPubKey;
//	int result = secp256k1_ec_pubkey_create(this->Context, &rawPubKey, PrivateKey.GetData());
//	if (result == 0) 
//	{
//		UE_LOG(LogTemp, Error, TEXT("CHROMA::secp256k1_ec_pubkey_create failed"));
//	}
//
//	// Serialize public key
//	size_t pubKeyLen = 33;
//	PublicKey.Init(0, pubKeyLen);
//	result = secp256k1_ec_pubkey_serialize(this->Context, PublicKey.GetData(), &pubKeyLen, &rawPubKey, SECP256K1_EC_COMPRESSED);
//	if (result == 0)
//	{
//		UE_LOG(LogTemp, Error, TEXT("CHROMA::secp256k1_ec_pubkey_serialize failed"));
//	}
//
//	return true;
//}
//
//
//FString PostchainUtil::KeyToString(const TArray<byte> &key)
//{
//	return FString(UTF8_TO_TCHAR(key.GetData()));
//}
//
//
//bool PostchainUtil::GenerateKeyPair(TMap<FString, TArray<byte>> &Out)
//{
//	TArray<byte> privKey;
//	if (!this->GeneratePrivateKey(privKey)) 
//	{
//		UE_LOG(LogTemp, Error, TEXT("CHROMA::GeneratePrivateKey failed"));
//		return false;
//	}
//	UE_LOG(LogTemp, Error, TEXT("CHROMA::PrivateKey: [%s]"), *(this->KeyToString(privKey)));
//
//	TArray<byte> pubKey;
//	if (!this->GenerateCompresedPublicKey(privKey, pubKey)) 
//	{
//		UE_LOG(LogTemp, Error, TEXT("CHROMA::GeneratePublicKey failed"));
//		return false;
//	}
//	UE_LOG(LogTemp, Error, TEXT("CHROMA::PubKey: [%s]"), *(this->KeyToString(pubKey)));
//
//	// Move values to out reference
//	Out.Add("privKey", privKey);
//	Out.Add("pubKey", pubKey);
//
//	return true;
//}
//
