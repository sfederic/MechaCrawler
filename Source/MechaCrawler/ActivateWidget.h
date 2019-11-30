// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActivateWidget.generated.h"

/**
 * 
 */
UCLASS()
class MECHACRAWLER_API UActivateWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FString useText;
};
