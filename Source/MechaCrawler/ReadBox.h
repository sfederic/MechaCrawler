#pragma once

#include "Containers/UnrealString.h" 
#include "ReadBox.generated.h"

USTRUCT()
struct FReadBox
{
    GENERATED_BODY()

    FString actorName;
    bool readDialogue;
};