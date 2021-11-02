////
////#include "Containers/UnrealString.h"
////
//#include "RequestRaw.h"
//
//URequestRaw::URequestRaw()
//{
//
//}
//
//
//void URequestRaw::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
//{
//	if (bWasSuccessful)
//	{
//		GLog->Log(Response->GetContentAsString());
//	}
//}
//
//
//void URequestRaw::SendHttpRequest(const FString& Url, const FString& RequestContent)
//{
//	//Creating a request using UE4's Http Module
//	TSharedRef<IHttpRequest,ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
//
//	//Binding a function that will fire when we receive a response from our request
//	Request->OnProcessRequestComplete().BindUObject(this, &URequestRaw::OnResponseReceived);
//
//	//This is the url on which to process the request
//	Request->SetURL(Url);
//	//We're sending data so set the Http method to POST
//	Request->SetVerb("POST");
//
//	//Tell the host that we're an unreal agent
//	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
//
//	//In case you're sending json you can also make use of headers using the following command. 
//		//Just make sure to convert the Json to string and use the SetContentAsString.
//	//Request->SetHeader("Content-Type", TEXT("application/json"));
//	//Use the following command in case you want to send a string value to the server
//	//Request->SetContentAsString("Hello kind server.");
//
//	//Send the request
//	Request->ProcessRequest();
//}
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
////
//////namespace chromia {
//////namespace postchain {
//////
////URequestRaw::URequestRaw(std::string uri)
////{
////	this->uri_ = uri;
////}
////
////URequestRaw::URequestRaw(std::string url_base, std::string path)
////{
////	this->uri_ = url_base + path; // TODO use Paths & URI utils
////	GETSync(&URequestRaw::GetResponse);
////}
////
////void URequestRaw::GETSync(TFunction<void(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)> Callback)
////{
////	/*TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
////
////	JsonObject->SetStringField(TEXT("some_string_field"), *FString::Printf(TEXT("%s"), *SomeFStringVariable));
////*/
////	/*;
////
////	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&OutputString);
////
////	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);*/
////
////	FHttpModule& module = FHttpModule::Get();
////	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = module.CreateRequest();
////
////	HttpRequest->SetVerb("GET");
////	HttpRequest->SetHeader("Content-Type", "application/json");
////	HttpRequest->SetURL(*FString::Printf(TEXT("%s"), uri_.c_str()));
////
////	/*FString OutputString;
////	HttpRequest->SetContentAsString(OutputString);*/
////
////	HttpRequest->OnProcessRequestComplete().BindUObject(this, Callback);
////
////
////	/*var request = new UnityWebRequest(this._uri, "GET");
////	request.downloadHandler = new DownloadHandlerBuffer();
////	yield return request.SendWebRequest();
////
////	if (!CheckError(request))
////	{
////		this.content = request.downloadHandler.text;
////	}*/
////}
////
////void URequestRaw::Post()
////{
////	GETSync(&URequestRaw::GetResponse);
////
////	/*var request = new UnityWebRequest(this._uri, "POST");
////	byte[] bodyRaw = System.Text.Encoding.UTF8.GetBytes(payload);
////	var uploader = new UploadHandlerRaw(bodyRaw);
////
////	uploader.contentType = "application/json";
////
////	request.uploadHandler = uploader;
////	request.downloadHandler = (DownloadHandler)new DownloadHandlerBuffer();
////
////	yield return request.SendWebRequest();
////
////	if (!CheckError(request))
////	{
////		this.content = request.downloadHandler.text;
////	}*/
////}
////
//////}  // namespace postchain
//////}  // namespace chromia