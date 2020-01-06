// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHealthBarWidget.generated.h"

/**
 * TODO: Put this all into Slate somehow

 * This sort of usage is fucked up. Runs like this:
 * - BP inherit from this class, bind the progress bar to 'health'
 * - Give the enemy actor class the usual widget creation
 * - Set the widget from the widgetcomponent
 */
UCLASS()
class MECHACRAWLER_API UEnemyHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float health;
};
