// Fill out your copyright notice in the Description page of Project Settings.

#include "Teleporter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Mecha.h"
#include "DrawDebugHelpers.h"

ATeleporter::ATeleporter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ATeleporter::BeginPlay()
{
	Super::BeginPlay();
	
	playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void ATeleporter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (actorToTeleportTo)
	{
		DrawDebugLine(GetWorld(), GetActorLocation(), actorToTeleportTo->GetActorLocation(), FColor::Red);
	}

	if (GetActorLocation().Equals(playerPawn->GetActorLocation(), 9.9f))
	{
		AMecha* player = Cast<AMecha>(playerPawn);
		if (actorToTeleportTo)
		{
			FVector teleportLoc = actorToTeleportTo->GetActorLocation();
			FQuat teleportRot = actorToTeleportTo->GetActorQuat();

			player->SetActorLocation(teleportLoc);
			player->nextLoc = teleportLoc;
			player->currentLoc = teleportLoc;

			//player->SetActorRotation(teleportRot);
			//player->currentRot = teleportRot;
			//player->nextRot = teleportRot;

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Teleport actor not set on %s\n"), *this->GetName());
		}
	}
}
