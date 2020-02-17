// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScanWidget.generated.h"

/**
 * Widget for scanning against the world.
 */
UCLASS()
class MECHACRAWLER_API UScanWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString scanEntry;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString scanNameEntry;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString dialogueName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* dialogueImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* defaultDialogueImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasDialouge = false;
};
