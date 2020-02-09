// Fill out your copyright notice in the Description page of Project Settings.

#include "DialogueBox.h"
#include "Components/BoxComponent.h"
#include "Mecha.h"
#include "DrawDebugHelpers.h"

ADialogueBox::ADialogueBox()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ADialogueBox::BeginPlay()
{
	Super::BeginPlay();
	
	box = FindComponentByClass<UBoxComponent>();
	box->OnComponentBeginOverlap.AddDynamic(this, &ADialogueBox::OnPlayerOverlap);

	if (bIsActivated)
	{
		box->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	else if (!bIsActivated)
	{
		box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
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

			//Handle dialogue boxes that daisey chain
			for (int i = 0; i < dialogueBoxesToActivate.Num(); i++)
			{
				dialogueBoxesToActivate[i]->Activate();
			}

			//Handle decorative actors
			for (int i = 0; i < actorsToActivate.Num(); i++)
			{
				actorsToActivate[i]->SetActorHiddenInGame(false);
			}

			//Handle in-level events to spawn
			for (int i = 0; i < eventActors.Num(); i++)
			{
				eventActors[i]->Activate();
			}
		}
	}
}

void ADialogueBox::Activate()
{
	bIsActivated = true;

	if (bIsActivated)
	{
		box->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	else if (!bIsActivated)
	{
		box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
