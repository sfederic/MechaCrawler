// Fill out your copyright notice in the Description page of Project Settings.

#include "GridActor.h"

AGridActor::AGridActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGridActor::BeginPlay()
{
	Super::BeginPlay();

	originalLoc = GetActorLocation();
}

void AGridActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
