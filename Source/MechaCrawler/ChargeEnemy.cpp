// Fill out your copyright notice in the Description page of Project Settings.

#include "ChargeEnemy.h"
#include "Engine/World.h"
#include "Mecha.h"
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
			AMecha* player = Cast<AMecha>(attackHit.GetActor());
			if (player)
			{
				if (player->nextLoc.Equals(player->GetActorLocation()))
				{
					nextLoc = player->GetActorLocation() - GetActorForwardVector() * 100.f;
					nextLoc.X = FMath::RoundToFloat(nextLoc.X);
					nextLoc.Y = FMath::RoundToFloat(nextLoc.Y);
					nextLoc.Z = FMath::RoundToFloat(nextLoc.Z);

					charging = true;
				}
			}
		}
	}

	if (charging)
	{
		SetActorLocation(FMath::VInterpConstantTo(GetActorLocation(), nextLoc, DeltaTime, chargeMoveSpeed));
		if (nextLoc == GetActorLocation())
		{
			charging = false;
		}
	}
	else if (!charging)
	{
		SetActorLocation(FMath::VInterpConstantTo(GetActorLocation(), initialLoc, DeltaTime, retractMoveSpeed));
	}
}
