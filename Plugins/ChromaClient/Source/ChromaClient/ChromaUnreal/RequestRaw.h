//#pragma once 
//
//#include "CoreMinimal.h"
//#include "Misc/Attribute.h"
//#include "HttpModule.h"
//#include "Interfaces/IHttpResponse.h"
//
//#include "RequestRaw.generated.h"
//
//UCLASS()
//class URequestRaw : public UBlueprintFunctionLibrary {
//
//	GENERATED_BODY()
//
//public:
//
//	URequestRaw();
//
//	/*Sends an http request*/
//	UFUNCTION(BlueprintCallable, Category = Http)
//		void SendHttpRequest(const FString& Url, const FString& RequestContent);
//
//	/*Called when the server has responded to our http request*/
//	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
//};
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
////	bool error_ = false;
////	std::string error_message_ = "";
////	std::string content_;
////
////	URequestRaw(std::string uri);
////
////	URequestRaw(std::string url_base, std::string path);
////
////	/**
////	* Make HTTP HET request in sync mode. 
////	* @param content - Reference to content received from request, if no error
////	* @returns int - HTTP request status code
////	*/
////	/*template<class SourceType>
////	void GETSync(std::string& content, typename FGetter::template TUObjectMethodDelegate_Const< SourceType >::FMethodPtr InMethodPtr);
////*/
////
////	void GETSync(TFunction<void(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)> Callback);
////
////	UFUNCTION()
////	void GetResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
////	{
////		FString resp = Response->GetContentAsString();
////		UE_LOG(LogTemp, Display, TEXT("GET resposne: %s"), *resp);
////
////		//if (bWasSuccessful && Response->GetContentType() == "application/json")
////		//{
////		//	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
////
////		//	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(Response->GetContentAsString());
////
////		//	FJsonSerializer::Deserialize(JsonReader, JsonObject);
////
////		//	SomeOtherVariable = JsonObject->GetStringField("some_response_field");
////
////		//}
////		//else
////		//{
////		//	// Handle error here
////		//}
////	}
////
////	void Post();
////
////	/*{
////		var request = new UnityWebRequest(this._uri, "GET");
////		request.downloadHandler = new DownloadHandlerBuffer();
////		yield return request.SendWebRequest();
////
////		if (!CheckError(request))
////		{
////			this.content = request.downloadHandler.text;
////		}
////	}*/
////
////protected:
////	std::string uri_;
////		
////};
//////}  // namespace postchain
//////}  // namespace chromia
