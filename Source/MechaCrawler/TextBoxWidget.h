// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TextBox.h"
#include "TextBoxWidget.generated.h"

/**
 * 
 */
UCLASS()
class MECHACRAWLER_API UTextBoxWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString text;	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* image;

	TArray<FTextBox*> textBoxRows;

	int textBoxIndex = 0;
	int scrollIndex = 0;

	float scrollTimer = 0.f;
	float scrollSpeed = 6.5f;

	bool bScrollFinished = false;
};
