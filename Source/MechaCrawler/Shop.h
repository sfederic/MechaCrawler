// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Activate.h"
#include "Blueprint/UserWidget.h"
#include "Shop.generated.h"

/**
* basically a trade shop that pops up a UI on Use()
*/

UCLASS()
class MECHACRAWLER_API AShop : public AActor, public IActivate
{
	GENERATED_BODY()
	
public:	
	AShop();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void Use() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> shopWidgetClass;

	UPROPERTY()
	UUserWidget* shopWidget;
};
