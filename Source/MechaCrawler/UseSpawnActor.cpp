// Fill out your copyright notice in the Description page of Project Settings.


#include "UseSpawnActor.h"

AUseSpawnActor::AUseSpawnActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AUseSpawnActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (bStartsActive == false)
	{
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
	}
	else if (bStartsActive == true)
	{
		SetActorHiddenInGame(false);
		SetActorEnableCollision(true);
	}
}

void AUseSpawnActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUseSpawnActor::Use()
{
	if (bStartsActive == false)
	{
		SetActorHiddenInGame(false);
		SetActorEnableCollision(true);
	}
	else if (bStartsActive == true)
	{
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
	}
}
