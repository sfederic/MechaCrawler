// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MainGameInstance.generated.h"

/**
 * Main Instance class (only need one)
 */
UCLASS()
class MECHACRAWLER_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector playerSpawnLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName levelLoadTag;
};
