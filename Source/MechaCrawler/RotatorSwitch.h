// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Activate.h"
#include "RotatorSwitch.generated.h"

//Rotates set actors on activate

UCLASS()
class MECHACRAWLER_API ARotatorSwitch : public AActor, public IActivate
{
	GENERATED_BODY()
	
public:	
	ARotatorSwitch();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void Use() override;

	class AMecha* player; //For stoping movement while things rotate

	UPROPERTY(EditAnywhere)
	TArray<AActor*> actorsToRotate;

	UPROPERTY(EditAnywhere)
	FRotator rotateAxis;

	FRotator previousRotation;

	UPROPERTY(EditAnywhere)
	float rotateSpeed;

	bool bRotating = false;
};
