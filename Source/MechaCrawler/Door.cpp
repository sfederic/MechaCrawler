// Fill out your copyright notice in the Description page of Project Settings.

#include "Door.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();

	currentLoc = GetActorLocation();
	nextLoc = currentLoc;
	openState = false;
}

void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	currentLoc = FMath::VInterpConstantTo(currentLoc, nextLoc, DeltaTime, openSpeed);

	SetActorLocation(currentLoc);
}

void ADoor::Activate()
{
	if (openState == false && currentLoc == nextLoc)
	{
		nextLoc += GetActorUpVector() * 100.f;
		openState = true;
	}
	else if (openState == true && currentLoc == nextLoc)
	{
		nextLoc -= GetActorUpVector() * 100.f;
		openState = false;
	}
}
