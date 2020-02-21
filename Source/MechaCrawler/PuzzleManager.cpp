// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzleManager.h"

APuzzleManager::APuzzleManager()
{
	PrimaryActorTick.bCanEverTick = true;

}

void APuzzleManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void APuzzleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (int i = 0; i < puzzleItems.Num(); i++)
	{
		if (puzzleItems[i]->bActivated == false)
		{
			return;
		}
	}

	Activate();
}

void APuzzleManager::Activate()
{
	UE_LOG(LogTemp, Warning, TEXT("Puzzle manager worked"));
}
