// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Ship.generated.h"

//Pawn used to travel world map

UCLASS()
class MECHACRAWLER_API AShip : public APawn
{
	GENERATED_BODY()

public:
	AShip();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void RotateRight(float val);
	void RotateLeft(float val);
	void Accelerate(float val);
	void Reverse(float val);

private:
	float velocityMin = 0.f;
	float velocityMax = 13.5f;

	float reverseVelocityMin = -13.5f;
	float reverseVelocityMax = 0.0f;

	UPROPERTY(VisibleAnywhere)
	float velocity;

	UPROPERTY(VisibleAnywhere)
	float reverseVelocity;

	UPROPERTY(EditAnywhere)
	float moveSpeed;	

	UPROPERTY(EditAnywhere)
	float reverseSpeed;
	
	UPROPERTY(EditAnywhere)
	float rotateSpeed;

	bool bReversing;
};
