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
			save->readDialogueBoxes.Add(dialogueBox->bHasBeenRead);
		}

		FString slotName = TEXT("Slot1");
		int32 slotIndex = 0;
		UGameplayStatics::SaveGameToSlot(save, slotName, slotIndex);

		UE_LOG(LogTemp, Warning, TEXT("Game Saved to %s at index %d\n"), *slotName, slotIndex);
	}
}

