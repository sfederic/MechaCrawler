// Fill out your copyright notice in the Description page of Project Settings.

#include "DialogueUse.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Mecha.h"
#include "DialogueComponent.h"

ADialogueUse::ADialogueUse()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ADialogueUse::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADialogueUse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADialogueUse::Use()
{
	APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	AMecha* player = Cast<AMecha>(playerPawn);
	if (player)
	{
		if (this->FindComponentByClass<UDialogueComponent>())
		{
			player->GetDialogue(this);
		}
	}
}

