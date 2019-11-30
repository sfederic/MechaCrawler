// Fill out your copyright notice in the Description page of Project Settings.

#include "Pushable.h"
#include "Kismet/GameplayStatics.h" 
#include "Mecha.h"

APushable::APushable()
{
	PrimaryActorTick.bCanEverTick = true;
	moveParams.AddIgnoredActor(this);
	moveDistance = 100.f;
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
		if (!GetWorld()->LineTraceSingleByChannel(moveHit, GetActorLocation(),
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
		APawn* pawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		AMecha* player = Cast<AMecha>(pawn);
		if (player)
		{
			FVector playerForward = player->forwardAxis;
			if (!GetWorld()->LineTraceSingleByChannel(moveHit, GetActorLocation(), GetActorLocation() + playerForward * moveDistance,
				ECC_WorldStatic, moveParams))
			{
				nextLoc += player->rootAxes[player->forwardAxisIndex] * moveDistance;
				nextLoc.X = FMath::RoundToFloat(nextLoc.X);
				nextLoc.Y = FMath::RoundToFloat(nextLoc.Y);
				nextLoc.Z = FMath::RoundToFloat(nextLoc.Z);
				UE_LOG(LogTemp, Warning, TEXT("Working"));
			}
		}
	}
}
