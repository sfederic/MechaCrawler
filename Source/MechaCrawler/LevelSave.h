// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MapSaveData.h"
#include "Memories.h"
#include "LevelSave.generated.h"

/**
 * Main save file. Called levelsave just because, holds globals too
 */

UCLASS()
class MECHACRAWLER_API ULevelSave : public USaveGame
{
	GENERATED_BODY()

public:

    //Level save data
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMapSaveData> levelData; //TMap too much hassle with USTRUCT() for keys

    //Global save data
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<EMemories> heldMemories;
};
