// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NoteWidget.generated.h"

/**
 * 
 */
UCLASS()
class MECHACRAWLER_API UNoteWidget : public UUserWidget
{
	GENERATED_BODY()
public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString note;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bFirstSpawn = true;
};
