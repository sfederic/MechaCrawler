#pragma once

#include "Containers/UnrealString.h" 
#include "ReadBox.generated.h"

//For the dialogue boxes
//Keep in mind that Uproperty() is needed.

USTRUCT(BlueprintType)
struct FReadBox
{
public:
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString actorName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool readDialogue;
};