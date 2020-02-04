// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DestructibleActor.h"
#include "DestructibleSwitch.h"
#include "DestructibleActivate.generated.h"

/**
 * Actor that works off of multiple DestructibleSwitches as supports (eg. roof, floor, net)
 */
UCLASS()
class MECHACRAWLER_API ADestructibleActivate : public ADestructibleActor
{
	GENERATED_BODY()
	
public:
	ADestructibleActivate();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	bool CheckAllSwitches();

	UPROPERTY(EditAnywhere)
	TArray<ADestructibleSwitch*> switches;

	UDestructibleComponent* dc;
};
