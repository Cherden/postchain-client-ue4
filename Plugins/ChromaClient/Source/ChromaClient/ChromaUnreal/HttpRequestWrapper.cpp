//#include "http_request.h"
//#include "../../ChromaUnreal/Utils.h"
//#include "ssl_request.h"
//
//namespace chromia {
//namespace postchain {
//namespace http {
//
//HttpRequest::HttpRequest(std::string url, std::function<void(int, std::string)> callback)
//{
//	this->url_ = url;
//	this->callback_ = callback;
//
//	//test_ssl_trequest(0, nullptr);
//}
//
//
//void HttpRequest::SetContent(std::string content)
//{
//	this->content_ = content;
//}
//
//
//void HttpRequest::Get()
//{
//	callback_(0, "");
//	//TSharedRef<IHttpRequest, ESPMode::ThreadSafe> request = FHttpModule::Get().CreateRequest();
//	//request->OnProcessRequestComplete().BindUObject(this, &HttpRequest::OnResponseReceived);
//	//request->SetURL(*(ChromaUtils::STDStringToFString(this->url_)));
//	//request->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent"));
//	//request->SetVerb("GET");
//	//request->SetContentAsString(*(ChromaUtils::STDStringToFString(this->content_)));
//	//request->ProcessRequest();
//}
//
//
//void HttpRequest::Post()
//{
//	callback_(0, "");
//	/*TSharedRef<IHttpRequest, ESPMode::ThreadSafe> request = FHttpModule::Get().CreateRequest();
//	request->OnProcessRequestComplete().BindUObject(this, &HttpRequest::OnResponseReceived);
//	request->SetURL(*(ChromaUtils::STDStringToFString(this->url_)));
//	request->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent"));
//	request->SetVerb("POST");
//	request->SetContentAsString(*(ChromaUtils::STDStringToFString(this->content_)));
//	request->ProcessRequest();*/
//}
//
//void HttpRequest::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
//{
//	if (bWasSuccessful){
//		callback_(Response->GetResponseCode(), ChromaUtils::FStringToSTDString(Response->GetContentAsString()));
//	}
//	else 
//	{
//		callback_(-1, "");
//	}
//}
//
//} // namespace http
//} // namespace postchain
//} // namespace chromia