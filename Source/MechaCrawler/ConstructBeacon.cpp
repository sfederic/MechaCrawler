// Fill out your copyright notice in the Description page of Project Settings.


#include "ConstructBeacon.h"
#include "GlobalTags.h"
#include "Components/MeshComponent.h"
#include "Kismet/GameplayStatics.h"

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

		if (useManager)
		{
			useManager->activatedItems.Add(this->activateOrderNumber);
		}

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), activateSound, GetActorLocation(), 2.f, 1.6f);

		Tags.Remove(Tags::ScanUse);
	}
}

void AConstructBeacon::Rebuild()
{
	FindComponentByClass<UMeshComponent>()->SetMaterial(0, originalMaterial);
	bActivated = false;
	Tags.Add(Tags::ScanUse);
}
