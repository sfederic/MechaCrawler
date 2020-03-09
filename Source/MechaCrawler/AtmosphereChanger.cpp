// Fill out your copyright notice in the Description page of Project Settings.

#include "AtmosphereChanger.h"
#include "Components/BoxComponent.h"
#include "Mecha.h"

AAtmosphereChanger::AAtmosphereChanger()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AAtmosphereChanger::BeginPlay()
{
	Super::BeginPlay();
	
	box = FindComponentByClass<UBoxComponent>();
	box->OnComponentBeginOverlap.AddDynamic(this, &AAtmosphereChanger::ChangeDistance);

	fog = FindComponentByClass<UAtmosphericFogComponent>();
}

void AAtmosphereChanger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	dist = fog->DistanceScale;

	if (bIncrementDensity)
	{
		fog->SetDistanceScale(fog->DistanceScale + distanceIncrement * FApp::GetDeltaTime());

		if (fog->DistanceScale > maxDistance)
		{
			bIncrementDensity = false;
		}
	}
}

void AAtmosphereChanger::ChangeDistance(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AMecha>())
	{
		bIncrementDensity = true;
	}
}
