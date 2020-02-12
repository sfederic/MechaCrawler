// Fill out your copyright notice in the Description page of Project Settings.

#include "LoadPoint.h"
#include "Kismet/GameplayStatics.h"
#include "LevelSave.h"
#include "DialogueBox.h"
#include "Engine/World.h"
#include "LevelSave.h"
#include "MapSaveData.h"
#include "Pickup.h"
#include "GlobalTags.h"

ALoadPoint::ALoadPoint()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ALoadPoint::BeginPlay()
{
	Super::BeginPlay();

	ULevelSave* load = Cast<ULevelSave>(UGameplayStatics::LoadGameFromSlot(SaveSlots::Slot1, SaveSlots::slotIndex));

	FString levelName = UGameplayStatics::GetCurrentLevelName(GetWorld());

	if (load == nullptr)
	{
		//load = Cast<ULevelSave>(UGameplayStatics::CreateSaveGameObject(ULevelSave::StaticClass()));
		//load->levelData.Add(FMapSaveData(levelName));

		UE_LOG(LogTemp, Warning, TEXT("Load file was null and not found.")); //What about the index?
		return;
	}

	FMapSaveData mapLoadData = {};

	for (int i = 0; i < load->levelData.Num(); i++)
	{
		if (load->levelData[i].levelName == levelName)
		{
			mapLoadData = load->levelData[i];
			goto loadLevel;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Load file not found.")); //What about the index?
	return;

	loadLevel:

	//ADialogueBoxes
	TArray<AActor*> dialogueBoxes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADialogueBox::StaticClass(), dialogueBoxes);
	for (int i = 0; i < dialogueBoxes.Num(); i++)
	{
		for (int j = 0; j < mapLoadData.readDialogueBoxes.Num(); j++)
		{
			ADialogueBox* dialogueBox = Cast<ADialogueBox>(dialogueBoxes[i]);

			if (mapLoadData.readDialogueBoxes[j].actorName.Equals(*dialogueBoxes[i]->GetName()))
			{
				dialogueBox->bHasBeenRead = mapLoadData.readDialogueBoxes[j].readDialogue;
			}
		}
	}

	//APickups
	TArray<AActor*> pickups;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APickup::StaticClass(), pickups);
	for (int i = 0; i < pickups.Num(); i++)
	{
		for (int j = 0; j < mapLoadData.pickups.Num(); j++)
		{
			APickup* pickup = Cast<APickup>(pickups[i]);

			if (mapLoadData.pickups[j].actorName.Equals(*pickups[i]->GetName()))
			{
				pickup->isInInventory = mapLoadData.pickups[j].bPickedup;
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Game %s loaded from index %d\n"), *SaveSlots::Slot1, SaveSlots::slotIndex);
}

void ALoadPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
