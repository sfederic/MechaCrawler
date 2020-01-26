// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzleBeamInteract.h"
#include "Components/StaticMeshComponent.h"
#include "GlobalTags.h"

APuzzleBeamInteract::APuzzleBeamInteract()
{
	PrimaryActorTick.bCanEverTick = true;

}

void APuzzleBeamInteract::BeginPlay()
{
	Super::BeginPlay();
	
}

void APuzzleBeamInteract::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bOn && onMaterial)
	{
		this->FindComponentByClass<UStaticMeshComponent>()->SetMaterial(0, onMaterial);

		if (bTransparent)
		{
			if (Tags.Contains(Tags::MoveThrough))
			{
				Tags.Remove(Tags::MoveThrough);
			}
		}
	}
	else if (!bOn && offMaterial)
	{
		if (bTransparent)
		{
			if (!Tags.Contains(Tags::MoveThrough))
			{
				Tags.Add(Tags::MoveThrough);
			}
		}

		this->FindComponentByClass<UStaticMeshComponent>()->SetMaterial(0, offMaterial);
	}
}
