// Fill out your copyright notice in the Description page of Project Settings.

#include "DialogueBox.h"
#include "Components/BoxComponent.h"
#include "Mecha.h"

ADialogueBox::ADialogueBox()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ADialogueBox::BeginPlay()
{
	Super::BeginPlay();
	
	box = FindComponentByClass<UBoxComponent>();
	box->OnComponentBeginOverlap.AddDynamic(this, &ADialogueBox::OnPlayerOverlap);
}

void ADialogueBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADialogueBox::OnPlayerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bHasBeenRead == false)
	{
		AMecha* player = Cast<AMecha>(OtherActor);
		if (player)
		{
			player->GetDialogue(this);
			bHasBeenRead = true;
		}
	}
}
