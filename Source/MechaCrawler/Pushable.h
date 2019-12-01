// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridActor.h"
#include "Activate.h"
#include "Scannable.h"
#include "Pushable.generated.h"

UCLASS()
class MECHACRAWLER_API APushable : public AGridActor, public IActivate, public IScannable
{
	GENERATED_BODY()
	
public:	
	APushable();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void Use() override;
	virtual UScanData* Scan() override;

	FHitResult moveHit;
	FCollisionQueryParams moveParams;

	UPROPERTY(EditAnywhere)
	float moveSpeed;

	float moveDistance;
};
