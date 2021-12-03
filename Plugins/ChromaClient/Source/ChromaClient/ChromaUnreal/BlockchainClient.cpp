#include "BlockchainClient.h"
#include "Engine/GameEngine.h"
#include "Misc/OutputDeviceDebug.h"
#include "Utils.h"
#include "../chroma-cpp-pure/src/postchain_util.h"
#include "../chroma-cpp-pure/HTTP/httprequest.h"

#include <string>      
#include <iostream>   
#include <sstream> 

//#include <curl/curl.h>

//#include "curl_easy.h"
//#include "curl_form.h"
//#include "curl_ios.h"
//#include "curl_exception.h"
//#include "curl_header.h"
//
//using curl::curl_header;
//using curl::curl_form;
//using curl::curl_easy;
//using curl::curl_pair;
//using curl::curl_easy_exception;
//using curl::curlcpp_traceback;

//#include <curlpp/cURLpp.hpp>
//#include <curlpp/Easy.hpp>
//#include <curlpp/Options.hpp>
//#include "../chroma-cpp-pure/HTTP/http-req-client.hpp"




//static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
//{
//	((std::string*)userp)->append((char*)contents, size * nmemb);
//	return size * nmemb;
//}



ABlockchainClient::ABlockchainClient(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

	std::string response = "";
	std::string error = "";
	UHttpRequest::SendGetRequestSync("http://localhost:3000/success", response, error);
	UE_LOG(LogTemp, Error, TEXT("Result: %s  %s"), *ChromaUtils::STDStringToFString(response), *ChromaUtils::STDStringToFString(error));
	
	response = "";
	error = "";
	UHttpRequest::SendGetRequestSync("https://localhost:3000/success", response, error);
	UE_LOG(LogTemp, Error, TEXT("Result: %s  %s"), *ChromaUtils::STDStringToFString(response), *ChromaUtils::STDStringToFString(error));
	
	response = "";
	error = "";
	UHttpRequest::SendGetRequestSync("http://unekistingjsddd000/444", response, error);
	UE_LOG(LogTemp, Error, TEXT("Result: %s  %s"), *ChromaUtils::STDStringToFString(response), *ChromaUtils::STDStringToFString(error));

	// ----- POST -----------------------------------------------------------------------------
	//CURL *curl;
	//CURLcode res;
	//std::string readBuffer;

	//curl = curl_easy_init();
	//if (curl) {
	//	curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:7740/query/849ad8c9ac720a21962187d0bda6168da274e1d17d39aad513559171fddc6914");
	//	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{\"type\" : \"q\"}");
	//	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	//	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
	//	res = curl_easy_perform(curl);
	//	curl_easy_cleanup(curl);

	//	//std::cout << readBuffer << std::endl;
	//}
	//UE_LOG(LogTemp, Error, TEXT("Result: %s"), *ChromaUtils::STDStringToFString(readBuffer));
	//-----------------------------------------------------------------------------------------------


	// ----- GET -----------------------------------------------------------------------------
	//CURL *curl;
	//CURLcode res;
	//std::string readBuffer;

	//curl = curl_easy_init();
	//if (curl) {
	//	curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:3000/success");
	//	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	//	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
	//	res = curl_easy_perform(curl);
	//	curl_easy_cleanup(curl);
	//	//std::cout << readBuffer << std::endl;
	//}
	//UE_LOG(LogTemp, Error, TEXT("Result: %s"), *ChromaUtils::STDStringToFString(readBuffer));
	//-----------------------------------------------------------------------------------------------

	//using namespace std::string_literals;

	//





	//// ---------------- GET ------------------------------
	//std::string url = "http://localhost:3000/success";

	//std::stringstream str;
	//try
	//{
	//	curl::curl_ios<std::stringstream> writer(str);
	//	curl::curl_easy easy(writer);

	//	easy.add<CURLOPT_URL>(url.data());
	//	easy.add<CURLOPT_FOLLOWLOCATION>(1L);

	//	easy.perform();
	//}
	//catch (curl::curl_easy_exception const & error)
	//{
	//	auto errors = error.get_traceback();
	//	error.print_traceback();
	//}

	//std::string result = str.str();
	//UE_LOG(LogTemp, Error, TEXT("Result: %s"), *ChromaUtils::STDStringToFString(result));


	//try
	//{
	//	// That's all that is needed to do cleanup of used resources (RAII style).
	//curlpp::Cleanup myCleanup;

	//	// Our request to be sent.
	//curlpp::Easy myRequest;

	//	// Set the URL.
	//myRequest.setOpt<Url>("http://example.com");

	//	// Send request and get a result.
	//	// By default the result goes to standard output.
	//myRequest.perform();
	
	//}
	//catch (curlpp::RuntimeError & e)
	//{
	//	std::cout << e.what() << std::endl;
	//}
	//catch (curlpp::LogicError & e)
	//{
	//	std::cout << e.what() << std::endl;
	//}


	/*try
	{*/
		// you can pass http::InternetProtocol::V6 to Request to make an IPv6 request
		//http::Request request{ "http://localhost:3000/success" };

		//// send a get request
		//auto response = request.send("GET");
		
	/*	UE_LOG(LogTemp, Warning, TEXT("CHROMA::success"));
	}
	catch (const std::exception& e)
	{
		UE_LOG(LogTemp, Warning, TEXT("CHROMA::error"));
	}*/


	if (!PostchainUtil::GenerateKeyPair(PrivateKey, PublicKey))
	{
		UE_LOG(LogTemp, Error, TEXT("CHROMA::UBlockchainClient::GenerateKeyPair failed"));
	}

	BlockchainClientPtr = std::make_shared<BlockchainClient>();
}


void ABlockchainClient::SetMainWidget(UObject* mw)
{
	this->MainWidget = mw;
}


void ABlockchainClient::Setup(FString blockchainRID, FString baseURL)
{
	BlockchainClientPtr->Setup(ChromaUtils::FStringToSTDString(blockchainRID), ChromaUtils::FStringToSTDString(baseURL));
}


void ABlockchainClient::RegisterUser(FString username)
{
	if (!KeyPairIsValid())
	{
		PrintLogOnScreen("RegisterUser fail. Missing KeyPair");
		return;
	}
	std::shared_ptr<PostchainTransaction> transaction = BlockchainClientPtr->NewTransaction(std::vector<std::vector<byte>> { PublicKey }, [this](std::string error) {
		PrintLogOnScreen(FString("Transaction failed: ")  + ChromaUtils::STDStringToFString(error));
	});

	std::shared_ptr<gtv::ArrayValue> operation_values = AbstractValueFactory::EmptyArray();
	operation_values->Add(AbstractValueFactory::Build(ChromaUtils::FStringToSTDString(username)));
	transaction->AddOperation(std::string("register_user"), operation_values);

	///*
	//	The blockchain hashes the operations to generate a transaction id (txid) which has to be unique.
	//	In order to prevent this colision, a "nop" operation can be attached which will be filtered by
	//	the blockchain.
	//*/
	std::shared_ptr<gtv::ArrayValue> nop_operation_values = AbstractValueFactory::EmptyArray();
	static int nonce = PostchainUtil::RandomIntInRange(0, 100000);
	nop_operation_values->Add(AbstractValueFactory::Build(std::to_string(nonce)));
	transaction->AddOperation(std::string("nop"), nop_operation_values);

	transaction->Sign(PrivateKey, PublicKey);

	transaction->PostAndWait([this, username] (std::string content) {
		PrintLogOnScreen("User " + username + " registered successfully.");
	});

	return;
}


void ABlockchainClient::CheckUser(FString username)
{
	std::vector<QueryObject> queryObjects;
	queryObjects.push_back(QueryObject("name", AbstractValueFactory::Build(ChromaUtils::FStringToSTDString(username))));

	BlockchainClientPtr->Query(
		"check_user",
		queryObjects, 
		[this, username] (std::string content) {
			FString result = content.compare("1") == 0 ? "true" : "false";
			PrintLogOnScreen("check_user: " + username + "  " + result);
		},
		[this](std::string error) {
			PrintLogOnScreen(FString("check_user failed, error: ") + ChromaUtils::STDStringToFString(error));
		}
	);
}


void ABlockchainClient::PrintLogOnScreen(FString message)
{
	if (MainWidget == nullptr) return;

	const FString command = FString::Printf(TEXT("PrintLog %s"), *message);
	FOutputDeviceDebug debug;
	MainWidget->CallFunctionByNameWithArguments(*command, debug, this, true);
}


bool ABlockchainClient::KeyPairIsValid()
{
	if (!(PrivateKey.size() == 32)) return false;
	if (!(PublicKey.size() == 33)) return false;
	return true;
}
