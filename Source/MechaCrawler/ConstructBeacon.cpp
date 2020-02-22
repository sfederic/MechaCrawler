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

	originalMaterial = FindComponentByClass<UMeshComponent>()->GetMaterial(0);
}

void AConstructBeacon::Use()
{
	FindComponentByClass<UMeshComponent>()->SetMaterial(0, activatedMaterial);
}

void AConstructBeacon::Rebuild()
{
	FindComponentByClass<UMeshComponent>()->SetMaterial(0, originalMaterial);
}
