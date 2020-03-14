// Fill out your copyright notice in the Description page of Project Settings.


#include "SightDaemon.h"
#include "Engine/World.h"
#include "Mecha.h"

ASightDaemon::ASightDaemon()
{
	PrimaryActorTick.bCanEverTick = true;
	sightParams.AddIgnoredActor(this);
}

void ASightDaemon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASightDaemon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorld()->LineTraceSingleByChannel(sightHit, GetActorLocation(), GetActorLocation() = GetActorForwardVector() * sightLength, ECC_WorldStatic, sightParams))
	{
		if (sightHit.GetActor()->IsA<AMecha>())
		{
			//TODO: some sort of burning attack? Post process
		}
	}
}

