// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ReadBox.h"
#include "Pickup.h"
#include "PickupStruct.h"
#include "LevelSave.generated.h"

/**
 * Test save for dialogue bools
 */
UCLASS()
class MECHACRAWLER_API ULevelSave : public USaveGame
{
	GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FReadBox> readDialogueBoxes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FPickupStruct> pickups;
};
