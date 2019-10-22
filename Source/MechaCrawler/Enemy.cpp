// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "DrawDebugHelpers.h"

float moveTimer;
FVector nextLoc;

FCollisionQueryParams params;
FHitResult hit;

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;

	params.AddIgnoredActor(this);
}


void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	nextLoc = GetActorLocation();
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	moveTimer += FApp::GetDeltaTime();

	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 125.f, FColor::Red);
	DrawDebugLine(GetWorld(), nextLoc, nextLoc - (GetActorUpVector() * 125.f), FColor::Purple);

	if (moveTimer > 1.5f)
	{
		moveTimer = 0.f;

		if (GetWorld()->LineTraceSingleByChannel(hit, GetActorLocation(), GetActorLocation() + (GetActorForwardVector() * 125.f), ECC_WorldStatic, params))
		{
			GLog->Logf(TEXT("enemy wall hit"));

			float randomRotations[4] = { 90.f, 180.f, 270.f, 360.f };
			int rotationIndex = (int)FMath::RandRange(0.f, 4.f);
			SetActorRotation(GetActorRotation() + FRotator(0.f, randomRotations[rotationIndex], 0.f));
		}
		else
		{
			redoMove:

			float randomRotations[4] = { 90.f, 180.f, 270.f, 360.f };
			int rotationIndex = (int)FMath::RandRange(0.f, 4.f);

			nextLoc = GetActorLocation() + GetActorForwardVector() * 100.f;
			nextLoc.X = FMath::RoundToFloat(nextLoc.X);
			nextLoc.Y = FMath::RoundToFloat(nextLoc.Y);
			nextLoc.Z = FMath::RoundToFloat(nextLoc.Z);
			SetActorRotation(GetActorRotation() + FRotator(0.f, randomRotations[rotationIndex], 0.f));

			if (!GetWorld()->LineTraceSingleByChannel(hit, nextLoc, nextLoc - GetActorUpVector() * 125.f, ECC_WorldStatic))
			{
				goto redoMove;
			}
		}
	}

	SetActorLocation(FMath::VInterpConstantTo(GetActorLocation(), nextLoc, DeltaTime, moveSpeed));
}
