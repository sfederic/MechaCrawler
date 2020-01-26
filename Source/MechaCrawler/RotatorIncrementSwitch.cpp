// Fill out your copyright notice in the Description page of Project Settings.

#include "RotatorIncrementSwitch.h"
#include "Mecha.h"
#include "Kismet/GameplayStatics.h"

ARotatorIncrementSwitch::ARotatorIncrementSwitch()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARotatorIncrementSwitch::BeginPlay()
{
	Super::BeginPlay();

	APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	player = Cast<AMecha>(playerPawn);
}

void ARotatorIncrementSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bRotating)
	{
		player->canMove = false;

		for (int i = 0; i < actorsToRotate.Num(); i++)
		{
			actorsToRotate[i]->SetActorRotation(FMath::QInterpConstantTo(actorsToRotate[i]->GetActorRotation().Quaternion(), actorsToRotate[i]->nextRot, DeltaTime, rotateSpeed));
			
			if (i == (actorsToRotate.Num() - 1))
			{
				if (actorsToRotate[i]->GetActorRotation().Quaternion().Equals(actorsToRotate[i]->nextRot))
				{
					bRotating = false;
					player->canMove = true;
				}
			}
		}
	}
}

void ARotatorIncrementSwitch::Use()
{
	if (bRotating == false)
	{
		bRotating = true;

		for (int i = 0; i < actorsToRotate.Num(); i++)
		{
			actorsToRotate[i]->nextRot *= FQuat(rotateVector, FMath::DegreesToRadians(rotateIncrement));
		}
	}
}
