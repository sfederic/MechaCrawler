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
	if (connectedActor)
	{
		DrawDebugLine(GetWorld(), GetActorLocation(), connectedActor->GetActorLocation(), FColor::Blue);
	}
}

void AActivateSwitch::Use()
{
	if (connectedActor)
	{
		IActivate* use = Cast<IActivate>(connectedActor);
		if (use)
		{
			use->Use();
		}
 	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No actor connected to %s"), *GetNameSafe(this));
	}
}
