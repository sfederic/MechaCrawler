// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Turret.generated.h"

UCLASS()
class MECHACRAWLER_API ATurret : public AActor
{
	GENERATED_BODY()
	
public:	
	ATurret();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	FCollisionQueryParams shootParams;

	UPROPERTY(VisibleAnywhere)
	AActor* target;

	UPROPERTY(EditAnywhere)
	UParticleSystem* laserParticleTemplate;

	UPROPERTY(EditAnywhere)
	USoundBase* shootSound;

	UPROPERTY(VisibleAnywhere)
	float attackTimer;

	UPROPERTY(EditAnywhere)
	float attackInterval;
};
