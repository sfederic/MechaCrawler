#pragma once

#include "Containers/UnrealString.h" 
#include "PickupStruct.generated.h"

//For any pickups/collectibles in save data

USTRUCT(BlueprintType)
struct FPickupStruct
{
public:
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString actorName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bPickedup;
};
