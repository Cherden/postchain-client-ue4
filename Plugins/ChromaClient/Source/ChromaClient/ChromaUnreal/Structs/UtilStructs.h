#pragma once

#include "CoreMinimal.h"

#include "UtilStructs.generated.h"

UCLASS()
class USavedAccount : public UObject
{
	GENERATED_BODY()

public:
	USavedAccount(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {};

    ~USavedAccount()
    {
        UE_LOG(LogTemp, Display, TEXT("CHROMA::USavedAccount::~USavedAccount()"));
    };

	UPROPERTY(BlueprintReadOnly)
		FString m_Username;

	UPROPERTY(BlueprintReadOnly)
		FString m_PrivKey;

	UPROPERTY(BlueprintReadOnly)
		FString m_AccountId;
};


USTRUCT()
struct FQueryObjectPair
{
    GENERATED_BODY()

public:
    FQueryObjectPair() {};

    FQueryObjectPair(FString _name, FString _content) : m_Name(_name), m_StrContent(_content)
    {
        m_isInt = false;
    };

    FQueryObjectPair(FString _name, int _content) : m_Name(_name), m_IntContent(_content)
    {
        m_isInt = true;
    };

    bool m_isInt = true;

    FString m_Name;
    FString m_StrContent;
    int m_IntContent;
};
