// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridActor.h"
#include "Activate.h"
#include "Scannable.h"
#include "Rebuild.h"
#include "Pushable.generated.h"

UCLASS()
class MECHACRAWLER_API APushable : public AGridActor, public IActivate, public IScannable, public IRebuild
{
	GENERATED_BODY()

public:
	APushable();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void Use() override;
	virtual void Rebuild() override;
	virtual UScanData* Scan() override;

	FHitResult moveHit;
	FCollisionQueryParams moveParams;

	FVector originalLoc; //For respawning pushables. 13 secret Herbs and Transforms

	UPROPERTY(EditAnywhere)
	float moveSpeed;

	UPROPERTY(EditAnywhere)
	float fallSpeed;

	float moveDistance;

	bool falling;

private:
	float maxFallDistance = 10000.f;
};
