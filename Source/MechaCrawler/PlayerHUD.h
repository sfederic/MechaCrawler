// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
//#include "ActivateWidget.h"
#include "PlayerHUD.generated.h"


UCLASS()
class MECHACRAWLER_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void DrawHUD() override;

	int textIndex;

	//UPROPERTY(EditAnywhere, Category = "Widgets")
	//TSubclassOf<UUserWidget> widgetClass;

	//UActivateWidget* activateWidget;
};
