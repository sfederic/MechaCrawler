// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelChange.h"
#include "Mecha.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "LevelSave.h"
#include "DialogueBox.h"

ALevelChange::ALevelChange()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ALevelChange::BeginPlay()
{
	Super::BeginPlay();
	box = FindComponentByClass<UBoxComponent>();
	box->OnComponentBeginOverlap.AddDynamic(this, &ALevelChange::OnPlayerOverlap);
}

void ALevelChange::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelChange::OnPlayerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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
			FReadBox readBox = {};
			readBox.actorName = dialogueBox->GetName();
			readBox.readDialogue = dialogueBox->bHasBeenRead;
			save->readDialogueBoxes.Add(readBox);
		}

		FString slotName = TEXT("Slot1");
		int32 slotIndex = 0;
		UGameplayStatics::SaveGameToSlot(save, slotName, slotIndex);

		UE_LOG(LogTemp, Warning, TEXT("Game Saved to %s at index %d\n"), *slotName, slotIndex);

		FLatentActionInfo latentInfo;
		UGameplayStatics::OpenLevel(GetWorld(), levelName);
	}
}

