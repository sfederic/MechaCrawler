// Fill out your copyright notice in the Description page of Project Settings.


#include "SavePoint.h"
#include "Kismet/GameplayStatics.h"
#include "Mecha.h"
#include "LevelSave.h"
#include "DialogueBox.h"
#include "Components/BoxComponent.h"
#include "Pickup.h"
#include "NoteNode.h"
#include "NoteSaveStruct.h"
#include "Ship.h"
#include "GlobalTags.h"

ASavePoint::ASavePoint()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ASavePoint::BeginPlay()
{
	Super::BeginPlay();
	
	box = FindComponentByClass<UBoxComponent>();
	box->OnComponentBeginOverlap.AddDynamic(this, &ASavePoint::OnPlayerOverlapBegin);
}

void ASavePoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASavePoint::OnPlayerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMecha* player = Cast<AMecha>(OtherActor);
	AShip* ship = Cast<AShip>(OtherActor);
	if (player || ship)
	{
		FMapSaveData mapSaveData = {};
		int levelSaveDataIndex = 0;
		FString levelName = UGameplayStatics::GetCurrentLevelName(GetWorld());

		ULevelSave* save = Cast<ULevelSave>(UGameplayStatics::LoadGameFromSlot(SaveSlots::Slot1, SaveSlots::slotIndex));
		if (save == nullptr)
		{
			save = Cast<ULevelSave>(UGameplayStatics::CreateSaveGameObject(ULevelSave::StaticClass()));
			save->levelData.Add(FMapSaveData(levelName));
		}

		//Save level name for Load Game
		save->levelToLoadInto = FName(*levelName);
		save->loadLocation = GetActorLocation();

		//Delete previouslevel
		for (int i = 0; i < save->levelData.Num(); i++)
		{
			if (save->levelData[i].levelName == levelName)
			{
				save->levelData.RemoveAt(i);
			}
		}


		TArray<AActor*> dialogueBoxes;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADialogueBox::StaticClass(), dialogueBoxes);

		for (int i = 0; i < dialogueBoxes.Num(); i++)
		{
			ADialogueBox* dialogueBox = Cast<ADialogueBox>(dialogueBoxes[i]);
			FReadBox readBox;
			readBox.actorName = dialogueBox->GetName();
			readBox.readDialogue = dialogueBox->bHasBeenRead;
			mapSaveData.readDialogueBoxes.Add(readBox);
		}


		TArray<AActor*> pickups;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APickup::StaticClass(), pickups);

		for (int i = 0; i < pickups.Num(); i++)
		{
			APickup* pickup = Cast<APickup>(pickups[i]);
			FPickupStruct pickupData;
			pickupData.actorName = *pickup->GetName();
			pickupData.bPickedup = pickup->isInInventory;
			mapSaveData.pickups.Add(pickupData);
		}


		TArray<AActor*> noteNodes;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANoteNode::StaticClass(), noteNodes);

		for (int i = 0; i < noteNodes.Num(); i++)
		{
			ANoteNode* noteNode = Cast<ANoteNode>(noteNodes[i]);
			FNoteSaveStruct noteSave;
			noteSave.noteText = noteNode->noteText;
			noteSave.noteLocation = noteNode->GetActorLocation();
			mapSaveData.noteNodes.Add(noteSave);
		}


		TArray<AActor*> taggedActors;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), Tags::Tagged, taggedActors);
		for (int i = 0; i < taggedActors.Num(); i++)
		{
			FTaggedActorSaveStruct tagSave;
			tagSave.actorName = *taggedActors[i]->GetName();
			tagSave.tagged = true;
			mapSaveData.actorsToTag.Add(tagSave);
		}


		//Memories
		save->heldMemories.Empty(); //TODO: Better way to do this

		if (player)
		{
			for (int i = 0; i < player->memories.Num(); i++)
			{
				save->heldMemories.Add(player->memories[i]);
			}
		}
		else if (ship)
		{
			for (int i = 0; i < ship->memories.Num(); i++)
			{
				save->heldMemories.Add(ship->memories[i]);
			}
		}


		mapSaveData.levelName = levelName;

		save->levelData.Add(mapSaveData);

		UGameplayStatics::SaveGameToSlot(save, SaveSlots::Slot1, SaveSlots::slotIndex);

		UE_LOG(LogTemp, Warning, TEXT("Game Saved to %s at index %d\n"), *SaveSlots::Slot1, SaveSlots::slotIndex);
	}
}

