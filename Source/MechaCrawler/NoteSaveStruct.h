#pragma once

#include "Containers/UnrealString.h"
#include "NoteNode.h"
#include "NoteSaveStruct.generated.h"

USTRUCT(BlueprintType)
struct FNoteSaveStruct
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString noteText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector noteLocation;
};