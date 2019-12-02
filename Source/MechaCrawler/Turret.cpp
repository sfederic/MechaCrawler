// Fill out your copyright notice in the Description page of Project Settings.

#include "Turret.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h" 

ATurret::ATurret()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATurret::BeginPlay()
{
	Super::BeginPlay();
}

void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (target)
	{
		DrawDebugLine(this->GetWorld(), GetActorLocation(), target->GetActorLocation(), FColor::Red);
	}

	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), target->GetActorLocation()));
}
