// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueChoiceWidget.generated.h"

/**
 * Your visual novel esque "Yes/No" widget. Thanks Golden Sun
 */
UCLASS()
class MECHACRAWLER_API UDialogueChoiceWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString choice1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString choice2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool choice1Taken = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool choice2Taken = false;
};
