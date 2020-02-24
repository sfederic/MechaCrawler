// Fill out your copyright notice in the Description page of Project Settings.


#include "RotatingGridActor.h"

ARotatingGridActor::ARotatingGridActor()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ARotatingGridActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARotatingGridActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARotatingGridActor::Rebuild()
{
	SetActorRotation(originalRot);
}
