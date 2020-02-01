// Fill out your copyright notice in the Description page of Project Settings.

#include "ActivateSwitch.h"
#include "DrawDebugHelpers.h"

AActivateSwitch::AActivateSwitch()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AActivateSwitch::BeginPlay()
{
	Super::BeginPlay();
	
}

void AActivateSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//TODO: Remove. For debug to show connected actor.
	for (int i = 0; i < connectedActors.Num(); i++)
	{
		DrawDebugLine(GetWorld(), GetActorLocation(), connectedActors[i]->GetActorLocation(), FColor::Blue);
	}
}

void AActivateSwitch::Use()
{
	for (int i = 0; i < connectedActors.Num(); i++)
	{
		IActivate* use = Cast<IActivate>(connectedActors[i]);
		if (use)
		{
			use->Use();
		}
 	}
}
