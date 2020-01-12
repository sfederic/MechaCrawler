// Fill out your copyright notice in the Description page of Project Settings.

#include "RebuildActor.h"
#include "Misc/App.h"

ARebuildActor::ARebuildActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARebuildActor::BeginPlay()
{
	Super::BeginPlay();
}

void ARebuildActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isRebuilding)
	{
		rebuildTimer += FApp::GetDeltaTime();

		if (rebuildTimer > 1.0f)
		{
			SetActorHiddenInGame(true);
		}
	}
}
