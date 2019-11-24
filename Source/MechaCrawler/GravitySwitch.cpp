// Fill out your copyright notice in the Description page of Project Settings.

#include "GravitySwitch.h"
#include "Components/ArrowComponent.h"

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

	//TODO: For in-editor visual. replace with something better later on.
	/*if (gravityArrow)
	{
		switch (gravity)
		{
		case EDirections::UP:
			gravityArrow->SetWorldRotation(FRotator(0.f, 90.f, 0.f));
		case EDirections::DOWN:
			gravityArrow->SetWorldRotation(FRotator(0.f, -90.f, 0.f));
		case EDirections::RIGHT:
			gravityArrow->SetWorldRotation(FRotator(0.f, 0.f, 90.f));
		case EDirections::LEFT:
			gravityArrow->SetWorldRotation(FRotator(0.f, 0.f, -90.f));
		case EDirections::FORWARD:
			gravityArrow->SetWorldRotation(FRotator(0.f, 0.f, 0.f));
		case EDirections::BACK:
			gravityArrow->SetWorldRotation(FRotator(0.f, 180.f, 0.f));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Arrow not set on %s. Gravity can't be set."), *GetNameSafe(this));
	}*/
}

void AGravitySwitch::Use()
{
	if (player)
	{
		if (player->nextRot.Equals(player->currentRot))
		{
			player->nextRot *= FQuat(gravityArrow->GetRightVector(), FMath::DegreesToRadians(-90.f));
			player->nextRot.Normalize();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player isn't set for GravitySwitch %s\n"), *GetNameSafe(this));
	}
}
