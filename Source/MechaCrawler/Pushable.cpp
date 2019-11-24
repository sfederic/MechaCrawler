// Fill out your copyright notice in the Description page of Project Settings.

#include "Pushable.h"
#include "Engine/World.h"

APushable::APushable()
{
	PrimaryActorTick.bCanEverTick = true;

	moveParams.AddIgnoredActor(this);
}

void APushable::BeginPlay()
{
	Super::BeginPlay();

	nextLoc = GetActorLocation();

	if (moveSpeed <= 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Move Speed not set on %s"), *GetNameSafe(this));
	}
}

void APushable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//FALLING/GRAVITY
	if (nextLoc.Equals(GetActorLocation()))
	{
		if (GetWorld()->LineTraceSingleByChannel(moveHit, GetActorLocation(),
			GetActorLocation() - GetActorUpVector() * (moveDistance + 25.f), ECC_WorldStatic, moveParams))
		{
			nextLoc -= GetActorUpVector() * moveDistance;
			nextLoc.X = FMath::RoundToFloat(nextLoc.X);
			nextLoc.Y = FMath::RoundToFloat(nextLoc.Y);
			nextLoc.Z = FMath::RoundToFloat(nextLoc.Z);
		}
	}

	SetActorLocation(FMath::VInterpConstantTo(GetActorLocation(), nextLoc, DeltaTime, moveSpeed));
}

void APushable::Use()
{
	if (nextLoc.Equals(GetActorLocation()))
	{
		if (!GetWorld()->LineTraceSingleByChannel(moveHit, GetActorLocation(), GetActorLocation() + GetActorForwardVector() * moveDistance,
			ECC_WorldStatic, moveParams))
		{
			nextLoc += GetActorForwardVector() * moveDistance;
			nextLoc.X = FMath::RoundToFloat(nextLoc.X);
			nextLoc.Y = FMath::RoundToFloat(nextLoc.Y);
			nextLoc.Z = FMath::RoundToFloat(nextLoc.Z);
		}
	}
}
