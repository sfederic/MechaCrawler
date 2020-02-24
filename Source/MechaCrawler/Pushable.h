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
	void SetPlayerMove();

	class AMecha* player;

	FHitResult moveHit;
	FCollisionQueryParams moveParams;

	FTimerHandle playerMoveTimer;

	FVector originalGravityVector;

	UPROPERTY(EditAnywhere)
	FVector gravityVector;

	UPROPERTY(EditAnywhere)
	float moveSpeed;

	UPROPERTY(EditAnywhere)
	float fallSpeed;

	float moveDistance;

	bool falling;

	bool bMoving = false;

	TArray<AActor*> allRotatingActorsInLevel;

private:
	float maxFallDistance = 10000.f;
};
