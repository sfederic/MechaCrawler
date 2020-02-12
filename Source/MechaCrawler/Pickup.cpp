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

	if (isInInventory)
	{
		SetActorHiddenInGame(true);
		SetActorTickEnabled(false);
	}
}

void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isInInventory)
	{
		SetActorHiddenInGame(true);
		SetActorTickEnabled(false);
	}
}

void APickup::AddToInventory()
{
	AMecha* player = Cast<AMecha>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (player)
	{
		player->inventoryWidget->mineralsHeld++;
		player->inventoryWidget->minerals = TEXT("Minerals: ");
		player->inventoryWidget->minerals.AppendInt(player->inventoryWidget->mineralsHeld);
		this->isInInventory = true;
		//Destroy();
	}
}

void APickup::Use()
{
	AddToInventory();
}
