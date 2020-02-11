// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnterLevelWidget.generated.h"

/**
 * Widget for entering levels in the overworld
 */

UCLASS()
class MECHACRAWLER_API UEnterLevelWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString levelName;
};
