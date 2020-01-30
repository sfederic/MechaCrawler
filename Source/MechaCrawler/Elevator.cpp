// Fill out your copyright notice in the Description page of Project Settings.

#include "Elevator.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Mecha.h"

AElevator::AElevator()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AElevator::BeginPlay()
{
	Super::BeginPlay();
	
	APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	player = Cast<AMecha>(playerPawn);
}

void AElevator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetActorLocation().Equals(nextLoc))
	{
		bMovedToNextLoc = true;
		player->canMove = true;
	}
	else 
	{
		player->canMove = false;
		bMovedToNextLoc = false;
		SetActorLocation(FMath::VInterpConstantTo(GetActorLocation(), nextLoc, DeltaTime, moveSpeed));
	}
}

void AElevator::Use()
{
	if (bMovedToNextLoc)
	{
		if (nextMoveActors[currentMoveActorIndex])
		{
			nextLoc = nextMoveActors[currentMoveActorIndex]->GetActorLocation();
			nextLoc.X = FMath::RoundToFloat(nextLoc.X);
			nextLoc.Y = FMath::RoundToFloat(nextLoc.Y);
			nextLoc.Z = FMath::RoundToFloat(nextLoc.Z);

			if (currentMoveActorIndex >= (nextMoveActors.Num() - 1))
			{
				currentMoveActorIndex = 0;
			}
			else
			{
				currentMoveActorIndex++;
			}
		}
	}
}

void AElevator::Rebuild()
{
	SetActorLocation(originalLoc);
	nextLoc = originalLoc;
}
