// Fill out your copyright notice in the Description page of Project Settings.

#include "ChargeEnemy.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

AChargeEnemy::AChargeEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AChargeEnemy::BeginPlay()
{
	Super::BeginPlay();
	initialLoc = GetActorLocation();
}

FColor chargingColor = FColor::Blue;

void AChargeEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugLine(GetWorld(), initialLoc, initialLoc + GetActorForwardVector() * attackDistance, FColor::Red);

	if (charging == false)
	{
		FHitResult attackHit;
		FCollisionQueryParams params;
		params.AddIgnoredActor(this);
		if (GetWorld()->LineTraceSingleByChannel(attackHit, GetActorLocation(),
			GetActorLocation() + GetActorForwardVector() * attackDistance, ECC_WorldStatic, params))
		{
			AActor* actor = attackHit.GetActor();
			if (actor)
			{
				nextLoc = actor->GetActorLocation();
				nextLoc.X = FMath::RoundToFloat(nextLoc.X);
				nextLoc.Y = FMath::RoundToFloat(nextLoc.Y);
				nextLoc.Z = FMath::RoundToFloat(nextLoc.Z);

				charging = true;
			}
		}
	}

	if (charging)
	{
		SetActorLocation(FMath::VInterpConstantTo(GetActorLocation(), nextLoc, DeltaTime, chargeMoveSpeed));
		chargingColor = FColor::Red;
	}
	else if (charging && (!initialLoc.Equals(GetActorLocation())))
	{
		SetActorLocation(FMath::VInterpConstantTo(GetActorLocation(), initialLoc, DeltaTime, retractMoveSpeed));
	}

	if (initialLoc.Equals(GetActorLocation()))
	{
		charging = false;
		chargingColor == FColor::Blue;
	}
}
