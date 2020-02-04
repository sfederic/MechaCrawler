// Fill out your copyright notice in the Description page of Project Settings.

#include "DestructibleActivate.h"
#include "DestructibleComponent.h"

ADestructibleActivate::ADestructibleActivate()
{
	this->PrimaryActorTick.bCanEverTick = true;
}

void ADestructibleActivate::BeginPlay()
{
	Super::BeginPlay();

	dc = FindComponentByClass<UDestructibleComponent>();
	check(dc);
}

void ADestructibleActivate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CheckAllSwitches())
	{
		dc->ApplyDamage(10000.f, this->GetActorLocation(), GetActorForwardVector(), 1000.f);
	}
}


bool ADestructibleActivate::CheckAllSwitches()
{
	for (int i = 0; i < switches.Num(); i++)
	{
		if (switches[i]->bDestroyed == false)
		{
			return false;
		}
	}

	return true;
}
