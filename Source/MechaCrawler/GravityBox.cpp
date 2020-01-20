// Fill out your copyright notice in the Description page of Project Settings.

#include "GravityBox.h"
#include "Engine/World.h"

AGravityBox::AGravityBox()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AGravityBox::BeginPlay()
{
	Super::BeginPlay();
	
	currentLoc = GetActorLocation();
	nextLoc = currentLoc;
}

void AGravityBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	currentLoc = GetActorLocation();

	if (currentLoc.Equals(nextLoc))
	{
		const float fallDistance = 100.f;
		FHitResult fallHit;
		if (!GetWorld()->LineTraceSingleByChannel(fallHit, GetActorLocation(), GetActorLocation() + (gravityDirection * fallDistance), ECC_WorldStatic))
		{
			nextLoc += gravityDirection * fallDistance;
		}
	}

	SetActorLocation(FMath::VInterpConstantTo(currentLoc, nextLoc, DeltaTime, fallSpeed));
}

