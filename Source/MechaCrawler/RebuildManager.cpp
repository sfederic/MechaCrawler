// Fill out your copyright notice in the Description page of Project Settings.

#include "RebuildManager.h"

ARebuildManager::ARebuildManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARebuildManager::BeginPlay()
{
	Super::BeginPlay();
}

void ARebuildManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UE_LOG(LogTemp, Warning, TEXT("Actors %d\n"), rebuildActors.Num());
	UE_LOG(LogTemp, Warning, TEXT("Meshes %d\n"), rebuildMeshes.Num());
	UE_LOG(LogTemp, Warning, TEXT("Materials %d\n"), rebuildMaterials.Num());
}
