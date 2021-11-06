/**
* cpp - http wrapper.
* this header is UE4 independent.
* TO-DO replace implementation with pure openssl lib, to make it UE4 full independent
*/

// TO-DO replace with openssl
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Interfaces/IHttpRequest.h"

#include "CoreMinimal.h"

#include "HttpRequestWrapper.generated.h"

using namespace std;

namespace chromia {
namespace postchain {
namespace http {

UCLASS()
class UHttpRequestWrapper : public UObject
{
	GENERATED_BODY()

public:
	UHttpRequestWrapper(std::string url, std::function<void(int, std::string)> callback);

	void SetContent(std::string content);

	void Get();

	void Post();

private:

	std::string url_;
	std::string content_;
	std::function<void(int, std::string)> callback_;

	/*Called when the server has responded to http request*/
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};

} // namespace http
} // namespace postchain
} // namespace chromia