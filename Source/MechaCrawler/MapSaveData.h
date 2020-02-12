#pragma once

#include "Runtime/Core/Public/Containers/Array.h"
#include "PickupStruct.h"
#include "ReadBox.h"
#include "MapSaveData.generated.h"

USTRUCT(BlueprintType)
struct FMapSaveData
{
	GENERATED_BODY()

	FMapSaveData() {};

	FMapSaveData(FString level)
	{
		levelName = level;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString levelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FReadBox> readDialogueBoxes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPickupStruct> pickups;
};