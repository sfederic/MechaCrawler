// Fill out your copyright notice in the Description page of Project Settings.

#include "GridActor.h"
#include "Mecha.h"

AGridActor::AGridActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGridActor::BeginPlay()
{
	Super::BeginPlay();

	originalLoc = GetActorLocation();
	nextLoc = GetActorLocation();
	currentLoc = nextLoc;
	currentRot = FQuat(GetActorRotation());
	nextRot = currentRot;
	originalRot = FQuat(GetActorRotation());
}

void AGridActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGridActor::OnPlayerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMecha* player = Cast<AMecha>(OtherActor);
	if (player)
	{
		if (player->lastLoc.Equals(nextLoc))
		{
			player->currentLoc = player->nextLoc;
			player->nextLoc = player->currentLoc;
		}
		else
		{
			player->currentLoc = player->lastLoc;
			player->nextLoc = player->lastLoc;
		}
	}
}
