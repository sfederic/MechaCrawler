// Fill out your copyright notice in the Description page of Project Settings.

#include "Pushable.h"
#include "Kismet/GameplayStatics.h" 
#include "Mecha.h"
#include "Runtime/Engine/Public/TimerManager.h"

APushable::APushable()
{
	PrimaryActorTick.bCanEverTick = true;
	moveParams.AddIgnoredActor(this);
	moveDistance = 100.f;
}

void APushable::BeginPlay()
{
	Super::BeginPlay();

	originalLoc = GetActorLocation();
	nextLoc = GetActorLocation();

	APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	player = Cast<AMecha>(playerPawn);

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
		bMoving = false;

		if (!GetWorld()->LineTraceSingleByChannel(moveHit, GetActorLocation(),
			GetActorLocation() + gravityVector * (moveDistance + 25.f), ECC_WorldStatic, moveParams))
		{
			nextLoc += gravityVector * moveDistance;
			nextLoc.X = FMath::RoundToFloat(nextLoc.X);
			nextLoc.Y = FMath::RoundToFloat(nextLoc.Y);
			nextLoc.Z = FMath::RoundToFloat(nextLoc.Z);

			falling = true;
		}
		else
		{
			falling = false;
		}
	}

	if (falling)
	{
		SetActorLocation(FMath::VInterpConstantTo(GetActorLocation(), nextLoc, DeltaTime, fallSpeed));
	}
	else if(!falling)
	{
		SetActorLocation(FMath::VInterpConstantTo(GetActorLocation(), nextLoc, DeltaTime, moveSpeed));
	}
}

void APushable::Use()
{
	if (nextLoc.Equals(GetActorLocation()))
	{
		FVector prevLoc = nextLoc;

		if (player)
		{
			FVector playerForward = player->forwardAxis;
			if (playerForward.Equals(FVector::UpVector))
			{
				return;
			}

			if (!GetWorld()->LineTraceSingleByChannel(moveHit, GetActorLocation(), GetActorLocation() + playerForward * moveDistance,
				ECC_WorldStatic, moveParams))
			{
				nextLoc += player->rootAxes[player->forwardAxisIndex] * moveDistance;
				nextLoc.X = FMath::RoundToFloat(nextLoc.X);
				nextLoc.Y = FMath::RoundToFloat(nextLoc.Y);
				nextLoc.Z = FMath::RoundToFloat(nextLoc.Z);

				bMoving = true;

				if (!GetWorld()->LineTraceSingleByChannel(moveHit, nextLoc, nextLoc - GetActorUpVector() * maxFallDistance, ECC_WorldStatic, moveParams))
				{
					nextLoc = prevLoc;
					return;
				}

				player->canMove = false;
				GetWorldTimerManager().SetTimer(playerMoveTimer, this, &APushable::SetPlayerMove, 0.5f, false); //Has to be in-time with moveSpeed
			}
		}
	}
}

UScanData* APushable::Scan()
{
	return FindComponentByClass<UScanData>();
}

void APushable::Rebuild()
{
	if (originalLoc.Equals(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation()) == false)
	{
		this->nextLoc = this->originalLoc;
		this->SetActorLocation(this->originalLoc);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Rebuild clashing with player location %s"), *this->GetName());
	}
}

void APushable::SetPlayerMove()
{
	UE_LOG(LogTemp, Warning, TEXT("done"));
	player->canMove = true;
}
