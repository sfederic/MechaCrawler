// Fill out your copyright notice in the Description page of Project Settings.


#include "SavePoint.h"
#include "Kismet/GameplayStatics.h"
#include "Mecha.h"
#include "LevelSave.h"
#include "DialogueBox.h"
#include "Components/BoxComponent.h"

ASavePoint::ASavePoint()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ASavePoint::BeginPlay()
{
	Super::BeginPlay();
	
	box = FindComponentByClass<UBoxComponent>();
	box->OnComponentBeginOverlap.AddDynamic(this, &ASavePoint::OnPlayerOverlapBegin);

	saveSlotLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
}

void ASavePoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASavePoint::OnPlayerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMecha* player = Cast<AMecha>(OtherActor);
	if (player)
	{
		ULevelSave* save = Cast<ULevelSave>(UGameplayStatics::CreateSaveGameObject(ULevelSave::StaticClass()));

		TArray<AActor*> dialogueBoxes;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADialogueBox::StaticClass(), dialogueBoxes);
		for (int i = 0; i < dialogueBoxes.Num(); i++)
		{
			ADialogueBox* dialogueBox = Cast<ADialogueBox>(dialogueBoxes[i]);
			FReadBox readBox;
			readBox.actorName = dialogueBox->GetName();
			readBox.readDialogue = dialogueBox->bHasBeenRead;
			save->readDialogueBoxes.Add(readBox);
		}

		TArray<AActor*> pickups;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APickup::StaticClass(), pickups);
		for (int i = 0; i < pickups.Num(); i++)
		{
			APickup* pickup = Cast<APickup>(pickups[i]);
			FPickupStruct pickupData;
			pickupData.actorName = *pickup->GetName();
			pickupData.bPickedup = pickup->isInInventory;
			save->pickups.Add(pickupData);
		}

		UGameplayStatics::SaveGameToSlot(save, saveSlotLevelName, saveSlotIndex);

		UE_LOG(LogTemp, Warning, TEXT("Game Saved to %s at index %d\n"), *saveSlotLevelName, saveSlotIndex);
	}
}

