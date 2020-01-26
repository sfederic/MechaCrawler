// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridActor.h"
#include "Activate.h"
#include "RotatorIncrementSwitch.generated.h"

//Just an aside case class for when need to rotate in Quaternion increments instead of back and forth

UCLASS()
class MECHACRAWLER_API ARotatorIncrementSwitch : public AActor, public IActivate
{
	GENERATED_BODY()
	
public:	
	ARotatorIncrementSwitch();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void Use() override;

	class AMecha* player;

	UPROPERTY(EditAnywhere)
	TArray<AGridActor*> actorsToRotate;

	UPROPERTY(EditAnywhere)
	FVector rotateVector;

	UPROPERTY(EditAnywhere)
	float rotateIncrement;

	UPROPERTY(EditAnywhere)
	float rotateSpeed;

	bool bRotating = false;
};
