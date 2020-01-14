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

	if (target == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("target not set in %s\n"), *GetNameSafe(this));
	}
}

void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (target)
	{
		DrawDebugLine(this->GetWorld(), GetActorLocation(), target->GetActorLocation(), FColor::Red);
	}

	if (target)
	{
		SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), target->GetActorLocation()));
	}
}
