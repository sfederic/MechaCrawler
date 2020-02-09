// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Activate.h"
#include "FadeToBlackWidget.h"
#include "Blueprint/UserWidget.h"
#include "BlackoutText.generated.h"

//Shit class. Need it just for the Use(). Just for when any fade to black text is needed

UCLASS()
class MECHACRAWLER_API ABlackoutText : public AActor, public IActivate
{
	GENERATED_BODY()
	
public:	
	ABlackoutText();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void Use() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UFadeToBlackWidget> fadeWidgetClass;

	UFadeToBlackWidget* fadeWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetAnimation* fadeAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString fadeOutText;
};
