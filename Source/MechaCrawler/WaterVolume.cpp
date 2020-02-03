// Fill out your copyright notice in the Description page of Project Settings.

#include "WaterVolume.h"
#include "Components/BoxComponent.h"
#include "Mecha.h"

UBoxComponent* waterBounds;
UStaticMeshComponent* meshComponent;

AWaterVolume::AWaterVolume()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AWaterVolume::BeginPlay()
{
	Super::BeginPlay();
	
	waterBounds = FindComponentByClass<UBoxComponent>();
	if (waterBounds)
	{
		waterBounds->OnComponentBeginOverlap.AddDynamic(this, &AWaterVolume::OnActorBeginOverlap);
		waterBounds->OnComponentEndOverlap.AddDynamic(this, &AWaterVolume::OnOverlapEnd);
	}
}

void AWaterVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWaterVolume::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void AWaterVolume::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}
