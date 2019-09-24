// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

const int movesPerTurn = 10;

FVector movePositions[movesPerTurn][movesPerTurn];

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	currentLoc = GetActorLocation();
	currentRot = FQuat(GetActorRotation());
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FCollisionShape box;
	box.SetBox(FVector(45.f));
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	for (int x = 0; x < movesPerTurn; x++)
	{
		for (int y = 0; y < movesPerTurn; y++)
		{
			if (GetWorld()->SweepTestByChannel(movePositions[x][y], movePositions[x][y], FQuat::Identity, ECC_WorldStatic, box, params))
			{
				DrawDebugBox(GetWorld(), movePositions[x][y], box.GetExtent(), FColor::Red);
			}
			else 
			{
				DrawDebugBox(GetWorld(), movePositions[x][y], box.GetExtent(), FColor::Blue);
			}
		}
	}
}

