// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SalvagePointPopupWidget.generated.h"

/**
 * 
 */
UCLASS()
class MECHACRAWLER_API USalvagePointPopupWidget : public UUserWidget
{
	GENERATED_BODY()
public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int currentSalvagePoints;
};
