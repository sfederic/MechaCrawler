// Fill out your copyright notice in the Description page of Project Settings.

#include "RebuildSwitch.h"
#include "Kismet/GameplayStatics.h"
#include "Mecha.h"
#include "RebuildManager.h"
#include "IceComponent.h"

ARebuildSwitch::ARebuildSwitch()
{
	PrimaryActorTick.bCanEverTick = true;
	rebuildTimer = 0.0f;
}

void ARebuildSwitch::BeginPlay()
{
	Super::BeginPlay();
	
	rebuildSwitchPlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	rebuildSwitchPlayer = Cast<AMecha>(rebuildSwitchPlayerPawn);
}

void ARebuildSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(rebuildSwitchPlayer && switchActivated == false)
	{
		if (rebuildSwitchPlayer->GetActorLocation().Equals(GetActorLocation()))
		{
			rebuildTimer += FApp::GetDeltaTime();
			rebuildSwitchPlayer->canMove = false; //TODO: need to make a visual post-pross effect for timer

			if (rebuildTimer > 1.0f)
			{
				RebuildAll();
				rebuildTimer = 0.f;
			}
		}
	}

	if (switchActivated == true && rebuildSwitchPlayer->GetActorLocation().Equals(GetActorLocation()) == false)
	{
		switchActivated = false;
	}
}

void ARebuildSwitch::RebuildAll()
{
	//FOR ALL GENERAL DEST. ACTORS
	rebuildSwitchPlayer->RebuildAllDestroyedActors();

	//FOR ICE BLOCK ACTORS
	TArray<AActor*> iceBlockActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADestructibleActor::StaticClass(), iceBlockActors);

	for (int i = 0; i < iceBlockActors.Num(); i++)
	{
		UIceComponent* iceBlock = iceBlockActors[i]->FindComponentByClass<UIceComponent>();
		if (iceBlock)
		{
			iceBlock->TurnToIce();
		}
	}

	rebuildSwitchPlayer->canMove = true;
	switchActivated = true;
}
