// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SwitchReceiver.generated.h"

UCLASS()
class MECHACRAWLER_API ASwitchReceiver : public AActor
{
	GENERATED_BODY()
	
public:	
	ASwitchReceiver();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	TArray<class ABlockSwitch*> switchActors;
};
