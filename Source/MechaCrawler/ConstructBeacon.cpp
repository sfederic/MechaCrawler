// Fill out your copyright notice in the Description page of Project Settings.


#include "ConstructBeacon.h"
#include "GlobalTags.h"
#include "Components/MeshComponent.h"

AConstructBeacon::AConstructBeacon()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AConstructBeacon::BeginPlay()
{
	Super::BeginPlay();
	
	Tags.Add(Tags::Useable);
}

void AConstructBeacon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AConstructBeacon::Use()
{
	if (bActivated == false)
	{
		FindComponentByClass<UMeshComponent>()->SetMaterial(0, activatedMaterial);
		bActivated = true;
		useManager->activatedItems.Add(this->activateOrderNumber);
	}
}

void AConstructBeacon::Rebuild()
{
	FindComponentByClass<UMeshComponent>()->SetMaterial(0, originalMaterial);
	bActivated = false;
}
