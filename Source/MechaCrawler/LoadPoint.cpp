// Fill out your copyright notice in the Description page of Project Settings.

#include "LoadPoint.h"
#include "Kismet/GameplayStatics.h"
#include "LevelSave.h"
#include "DialogueBox.h"
#include "Engine/World.h"

ALoadPoint::ALoadPoint()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ALoadPoint::BeginPlay()
{
	Super::BeginPlay();

	loadSlotLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());

	ULevelSave* load = Cast<ULevelSave>(UGameplayStatics::LoadGameFromSlot(loadSlotLevelName, loadSlotIndex));
	if (load)
	{
		//ADialogueBoxes
		TArray<AActor*> dialogueBoxes;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADialogueBox::StaticClass(), dialogueBoxes);
		for (int i = 0; i < dialogueBoxes.Num(); i++)
		{
			for (int j = 0; j < load->readDialogueBoxes.Num(); j++)
			{
				ADialogueBox* dialogueBox = Cast<ADialogueBox>(dialogueBoxes[i]);

				if (load->readDialogueBoxes[j].actorName.Equals(*dialogueBoxes[i]->GetName()))
				{
					dialogueBox->bHasBeenRead = load->readDialogueBoxes[j].readDialogue;
				}
			}
		}

		//APickups
		TArray<AActor*> pickups;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APickup::StaticClass(), pickups);
		for (int i = 0; i < pickups.Num(); i++)
		{
			for (int j = 0; j < load->pickups.Num(); j++)
			{
				APickup* pickup = Cast<APickup>(pickups[i]);

				if (load->pickups[j].actorName.Equals(*pickups[i]->GetName()))
				{
					pickup->isInInventory = load->pickups[j].bPickedup;
				}
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("Game %s loaded from index %d\n"), *loadSlotLevelName, loadSlotIndex);
	}
}

void ALoadPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
