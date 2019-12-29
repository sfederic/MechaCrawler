// Fill out your copyright notice in the Description page of Project Settings.

#include "RebuildSwitch.h"
#include "Kismet/GameplayStatics.h"
#include "Mecha.h"
#include "RebuildManager.h"

ARebuildSwitch::ARebuildSwitch()
{
	PrimaryActorTick.bCanEverTick = true;

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
			rebuildSwitchPlayer->RebuildAllDestroyedActors();

			TArray<AActor*> iceBlockActors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AIceBlock::StaticClass(), iceBlockActors);

			for (int i = 0; i < iceBlockActors.Num(); i++)
			{
				AIceBlock* iceBlock = Cast<AIceBlock>(iceBlockActors[i]);
				if (iceBlock)
				{
					iceBlock->TurnToIce();
				}
			}

			switchActivated = true;
		}
	}

	if (switchActivated == true && rebuildSwitchPlayer->GetActorLocation().Equals(GetActorLocation()) == false)
	{
		switchActivated = false;
	}
}
