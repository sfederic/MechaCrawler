// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DestructibleActor.h"
#include "DestructibleSwitch.h"
#include "Rebuild.h"
#include "DestructibleActivate.generated.h"

/**
 * Actor that works off of multiple DestructibleSwitches as supports (eg. roof, floor, net)
 * Looks like we can only have one per level too, unless you generate a tag that matches an original tag of the instance
 */
UCLASS()
class MECHACRAWLER_API ADestructibleActivate : public ADestructibleActor, public IRebuild
{
	GENERATED_BODY()
	
public:
	ADestructibleActivate();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void Rebuild() override;
	bool CheckAllSwitches();

	UPROPERTY(EditAnywhere)
	TArray<ADestructibleSwitch*> switches;

	UPROPERTY(EditAnywhere)
	class ARebuildManager* rebuildManager;
};
