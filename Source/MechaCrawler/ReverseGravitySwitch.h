// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Activate.h"
#include "ReverseGravitySwitch.generated.h"

//For reversing Pushable actors

UCLASS()
class MECHACRAWLER_API AReverseGravitySwitch : public AActor, public IActivate
{
	GENERATED_BODY()
	
public:	
	AReverseGravitySwitch();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void Use() override;

	UPROPERTY()
	TArray<class APushable*> pushActors;
};
