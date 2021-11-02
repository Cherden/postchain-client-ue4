#pragma once

#include "CoreMinimal.h"
#include "../Chroma/common.h"
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

//
//
//namespace chromia {
//namespace postchain {
//
//// Forward declaration
//class Transaction;
//
//namespace client {
//
//	class BlockchainClient {
//	public:
//
//		///<summary>
//		///Sets parameter to connect to blockchain.
//		///</summary>
//		///<param name = "blockchainRID">The blockchain RID of the dapp.</param>
//		///<param name = "baseURL">Location of the blockchain.</param>
//		void Setup(std::string blockchain_rid, std::string base_url);
//
//		///<summary>
//	   ///Create a new Transaction.
//	   ///</summary>
//	   ///<param name = "signers">Array of signers. Can be empty and set later.</param>
//	   ///<param name = "onError">Action that gets called in case of any error with the transaction.</param>
//	   ///<returns>New PostchainTransaction object.</returns>
//		std::shared_ptr<Transaction> NewTransaction(std::vector<std::vector<unsigned char>> signers); //TO-DO add onError callback
//
//		 ///<summary>
//		///Queries data from the blockchain.
//		///</summary>
//		///<param name = "queryName">Name of the query in RELL.</param>
//		///<param name = "queryObject">Parameters of the query.</param>
//		///<param name = "onSuccess">Action that gets called when the query succeeds. Passes return value as parameter.</param>
//		///<param name = "onError">Action that gets called if any error occures while querying from blockchain. Passes error message as parameter.</param>
//		///<returns>Unity coroutine enumerator.</returns>
//		//void Query(std::string queryName, void* queryObject); //TO-DO add callbacks
//
//	private:
//		std::string blockchain_rid_;
//		int chain_id_;
//		std::string base_url_;
//	
//		void InitializeBRIDFromChainID();
//	};
//} // namespace client
//} // namespace postchain
//} // namespace chromia
//
//#endif