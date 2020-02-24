// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Activate.h"
#include "HiddenActivateActor.generated.h"

//Just an actor that will start hidden or become hidden on Use()

UCLASS()
class MECHACRAWLER_API AHiddenActivateActor : public AActor, public IActivate
{
	GENERATED_BODY()
	
public:	
	AHiddenActivateActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void Use() override;

	UPROPERTY(EditAnywhere)
	float fadeSpeed;

	float fadeIncrement = 0.f;
	bool bIsHidden = true;
};
