// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Activate.h"
#include "Mecha.h"
#include "Pushable.generated.h"

UCLASS()
class MECHACRAWLER_API APushable : public AActor, public IActivate
{
	GENERATED_BODY()
	
public:	
	APushable();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void Use() override;

	FHitResult moveHit;
	FCollisionQueryParams moveParams;

	UPROPERTY(EditAnywhere)
	AMecha* player;

	UPROPERTY(VisibleAnywhere)
	FVector nextLoc;

	UPROPERTY(EditAnywhere)
	float moveSpeed;

	float moveDistance = 100.f;
};
