// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MapSaveData.h"
#include "LevelSave.generated.h"

/**
 * Test save for dialogue bools
 */
UCLASS()
class MECHACRAWLER_API ULevelSave : public USaveGame
{
	GENERATED_BODY()

public:

    TArray<FMapSaveData> levelData; //TMap too much hassle with USTRUCT() for keys
};
