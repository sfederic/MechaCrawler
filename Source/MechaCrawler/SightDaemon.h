// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SightDaemon.generated.h"

UCLASS()
class MECHACRAWLER_API ASightDaemon : public AActor
{
	GENERATED_BODY()
	
public:	
	ASightDaemon();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	FHitResult sightHit;
	FCollisionQueryParams sightParams;

	UPROPERTY(EditAnywhere)
	float sightLength;
};
