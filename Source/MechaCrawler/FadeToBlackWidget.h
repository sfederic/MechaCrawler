// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FadeToBlackWidget.generated.h"

/**
 * 
 */
UCLASS()
class MECHACRAWLER_API UFadeToBlackWidget : public UUserWidget
{
	GENERATED_BODY()
public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString text;
};
