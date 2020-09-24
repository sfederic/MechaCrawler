// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelHider.h"
#include "Kismet/GameplayStatics.h"

ALevelHider::ALevelHider()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALevelHider::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALevelHider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bRevealLevel)
	{
		TArray<AActor*> attachedActors;
		GetAttachedActors(attachedActors);

		for (int i = 0; i < attachedActors.Num(); i++)
		{
			attachedActors[i]->SetActorHiddenInGame(true);
			attachedActors[i]->SetActorEnableCollision(false);
		}

		bRevealLevel = false;
	}
}

