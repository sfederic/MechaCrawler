#pragma once

#include "Containers/UnrealString.h"
#include "taggedActorSaveStruct.generated.h"

USTRUCT(BlueprintType)
struct FTaggedActorSaveStruct
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString actorName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool tagged;
};