// Fill out your copyright notice in the Description page of Project Settings.

#include "GravitySwitch.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Mecha.h"

UArrowComponent* gravityArrow;

AGravitySwitch::AGravitySwitch()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGravitySwitch::BeginPlay()
{
	Super::BeginPlay();
	
	gravityArrow = FindComponentByClass<UArrowComponent>();
}

void AGravitySwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (gravityArrow)
	{
		switch (gravity)
		{
		case EDirections::UP:
			gravityArrow->SetWorldRotation(FVector(0.f, 0.f, 1.f).ToOrientationRotator());
		case EDirections::DOWN:
			gravityArrow->SetWorldRotation(FVector(0.f, 0.f, -1.f).ToOrientationRotator());
		case EDirections::RIGHT:
			gravityArrow->SetWorldRotation(FVector(0.f, 1.f, 0.f).ToOrientationRotator());
		case EDirections::LEFT:
			gravityArrow->SetWorldRotation(FVector(0.f, -1.f, 0.f).ToOrientationRotator());
		case EDirections::FORWARD:
			gravityArrow->SetWorldRotation(FVector(1.f, 0.f, 0.f).ToOrientationRotator());
		case EDirections::BACK:
			gravityArrow->SetWorldRotation(FVector(-1.f, 0.f, 0.f).ToOrientationRotator());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Arrow not set on %s. Gravity can't be set."), *GetNameSafe(this));
	}
}

void AGravitySwitch::Use()
{
	APawn* pawn = UGameplayStatics::GetPlayerPawn(this->GetWorld(), 0);
	AMecha* player = Cast<AMecha>(pawn);
	if (player)
	{
		if (player->nextRot.Equals(player->currentRot))
		{
			player->nextRot *= FQuat(gravityArrow->GetRightVector(), FMath::DegreesToRadians(-90.f));
			player->nextRot.Normalize();
		}
	}
}
