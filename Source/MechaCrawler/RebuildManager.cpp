// Fill out your copyright notice in the Description page of Project Settings.

#include "RebuildManager.h"
#include "Kismet/GameplayStatics.h"
#include "Mecha.h"
#include "Door.h"
#include "Materials/MaterialInstanceDynamic.h"

ARebuildManager::ARebuildManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARebuildManager::BeginPlay()
{
	Super::BeginPlay();

	APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	player = Cast<AMecha>(playerPawn);
	check(player);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGridActor::StaticClass(), movingGridActors);
	UE_LOG(LogTemp, Warning, TEXT("NUMBER OF MOVING ACTORS: %d"), movingGridActors.Num());

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APushable::StaticClass(), pushableActors);
	UE_LOG(LogTemp, Warning, TEXT("NUMBER OF PUSHABLE ACTORS: %d"), pushableActors.Num());
}

void ARebuildManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARebuildManager::RebuildTimers()
{
	//REBUILD ACTOR TIMERS: Basically a cheap way to get around UE4's timers
	if (rebuildActors.Num() > 0 && rebuildTimers.Num() > 0)
	{
		for (int i = 0; i < rebuildActors.Num(); i++)
		{
			if (rebuildTimers[i] > 5.0f)
			{
				rebuildActors[i]->SetActorHiddenInGame(true);
				rebuildActors[i]->SetActorEnableCollision(false);
				rebuildActors[i]->SetActorTickEnabled(false);
			}
			else
			{
				rebuildTimers[i] += FApp::GetDeltaTime();
			}
		}
	}
}

void ARebuildManager::RebuildPushables()
{
	for (int i = 0; i < pushableActors.Num(); i++)
	{
		APushable* pushActor = Cast<APushable>(pushableActors[i]);
		if (pushActor)
		{
			if (pushActor->originalLoc.Equals(player->GetActorLocation()) == false)
			{
				pushActor->nextLoc = pushActor->originalLoc;
				pushActor->SetActorLocation(pushActor->originalLoc);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Rebuild clashing with player location %s"), *pushActor->GetName());
			}
		}
	}
}

void ARebuildManager::RebuildDoors()
{
	TArray<AActor*> doorActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADoor::StaticClass(), doorActors);

	for (int i = 0; i < doorActors.Num(); i++)
	{
		ADoor* door = Cast<ADoor>(doorActors[i]);
		if (door)
		{
			door->currentLoc = door->originalLoc;
			door->nextLoc = door->originalLoc;
			door->openState = false;
		}
	}
}

