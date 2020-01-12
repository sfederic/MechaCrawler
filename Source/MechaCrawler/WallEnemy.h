// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridActor.h"
#include "RebuildManager.h"
#include "WallEnemy.generated.h"

UCLASS()
class MECHACRAWLER_API AWallEnemy : public AGridActor
{
	GENERATED_BODY()
	
public:	
	AWallEnemy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	FCollisionQueryParams moveParams;

	FVector moveAxes[4];

	UPROPERTY(EditAnywhere)
	ARebuildManager* rebuildManager;

	UPROPERTY(EditAnywhere)
	float moveSpeed;

	float moveDistance = 100.f;

	UPROPERTY(VisibleAnywhere)
	float moveTimer = 0.f;
};
