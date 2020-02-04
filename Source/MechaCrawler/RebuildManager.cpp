// Fill out your copyright notice in the Description page of Project Settings.

#include "RebuildManager.h"
#include "Kismet/GameplayStatics.h"
#include "Mecha.h"
#include "Door.h"
#include "Components/StaticMeshComponent.h"
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
	//REBUILD ACTOR TIMERS: Basically a cheap  (lazy) way to get around UE4's timers

	for (int i = 0; i < rebuildActors.Num(); i++)
	{
		if (rebuildTimers[i] > 0.f && rebuildTimers[i] < 1.0f)
		{
			rebuildActors[i]->FindComponentByClass<UDestructibleComponent>()->SetScalarParameterValueOnMaterials("FadeValue", 1.f - rebuildTimers[i]);
		}

		if (rebuildTimers[i] > 1.0f)
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

	for (int i = 0; i < rebuildActorsActivate.Num(); i++)
	{
		if (rebuildActivateTimers[i] > 0.f && rebuildActivateTimers[i] < 1.0f)
		{
			rebuildActorsActivate[i]->FindComponentByClass<UDestructibleComponent>()->SetScalarParameterValueOnMaterials("FadeValue", 1.f - rebuildActivateTimers[i]);
		}

		if (rebuildActivateTimers[i] > 1.0f)
		{
			rebuildActorsActivate[i]->SetActorHiddenInGame(true);
			rebuildActorsActivate[i]->SetActorEnableCollision(false);
			rebuildActorsActivate[i]->SetActorTickEnabled(false);
		}
		else
		{
			rebuildActivateTimers[i] += FApp::GetDeltaTime();
		}
	}
}
