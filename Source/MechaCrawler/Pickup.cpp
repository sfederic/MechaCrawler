// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickup.h"
#include "Mecha.h"
#include "Kismet/GameplayStatics.h"

APickup::APickup()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APickup::BeginPlay()
{
	Super::BeginPlay();
}

void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickup::AddToInventory()
{
	AMecha* player = Cast<AMecha>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (player)
	{
		player->inventoryWidget->entries.Add(name);
		this->isInInventory = true;
		Destroy();
	}
}

void APickup::Use()
{
	AddToInventory();
}
