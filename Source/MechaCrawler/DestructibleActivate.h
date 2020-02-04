// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DestructibleActor.h"
#include "DestructibleSwitch.h"
#include "Activate.h"
#include "DestructibleActivate.generated.h"

/**
 * Actor that works off of multiple DestructibleSwitches as supports (eg. roof, floor, net)
 */
UCLASS()
class MECHACRAWLER_API ADestructibleActivate : public ADestructibleActor, public IActivate
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	bool CheckAllSwitches();
	virtual void Use() override;

	UPROPERTY(EditAnywhere)
	TArray<ADestructibleSwitch*> switches;

	UDestructibleComponent* dc;
};
