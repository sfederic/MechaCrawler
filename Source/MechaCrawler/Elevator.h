// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridActor.h"
#include "Activate.h"
#include "Rebuild.h"
#include "Elevator.generated.h"

//Different from the gravity box as the elevator will move to a designated actor(s)
//TODO: Does the elevator need a manual linetrace check against nextMoveActor? Can we use multiple moveActors?

UCLASS()
class MECHACRAWLER_API AElevator : public AGridActor, public IActivate, public IRebuild
{
	GENERATED_BODY()
	
public:	
	AElevator();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void Use() override;
	virtual void Rebuild() override;

	class AMecha* player;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> nextMoveActors;

	UPROPERTY(VisibleAnywhere)
	int currentMoveActorIndex = 0;

	UPROPERTY(EditAnywhere)
	float moveSpeed;

	bool bMovedToNextLoc;
};
