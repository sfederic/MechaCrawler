// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridActor.h"
#include "ChargeEnemy.generated.h"

/*
* Enemy that charges the player. Works like a Piston. Charges then returns to intial position when seeing player.
*/

UCLASS()
class MECHACRAWLER_API AChargeEnemy : public AGridActor
{
	GENERATED_BODY()
	
public:	
	AChargeEnemy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	FVector initialLoc;

	UPROPERTY(EditAnywhere)
	float attackDistance;

	UPROPERTY(EditAnywhere)
	float chargeMoveSpeed;

	UPROPERTY(EditAnywhere)
	float retractMoveSpeed;

	bool charging = false;
};
