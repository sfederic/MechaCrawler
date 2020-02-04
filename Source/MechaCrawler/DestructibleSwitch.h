// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DestructibleActor.h"
#include "Rebuild.h"
#include "DestructibleSwitch.generated.h"

/**
 * For actors that hold up other destructibles (eg. pillars, beams)
 */
UCLASS()
class MECHACRAWLER_API ADestructibleSwitch : public ADestructibleActor, public IRebuild
{
	GENERATED_BODY()
	
public:
	virtual void Rebuild() override;

	UPROPERTY(VisibleAnywhere)
	bool bDestroyed = false;
};
