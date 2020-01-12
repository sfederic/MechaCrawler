// Fill out your copyright notice in the Description page of Project Settings.

#include "RebuildManager.h"
#include "Kismet/GameplayStatics.h"

ARebuildManager::ARebuildManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARebuildManager::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGridActor::StaticClass(), movingGridActors);
	UE_LOG(LogTemp, Warning, TEXT("NUMBER OF ACTORS: %d"), movingGridActors.Num());
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
