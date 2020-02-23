// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TagDistanceWidget.generated.h"

/**
 * To show set distance from a tagged actor in world
 */
UCLASS()
class MECHACRAWLER_API UTagDistanceWidget : public UUserWidget
{
	GENERATED_BODY()
public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    AActor* taggedActor; //for tagged objects that move around, can update the text with this

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float distanceToPlayer;
};
