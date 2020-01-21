// Fill out your copyright notice in the Description page of Project Settings.

#include "RebuildSwitch.h"
#include "Kismet/GameplayStatics.h"
#include "Mecha.h"
#include "RebuildManager.h"
#include "IceComponent.h"
#include "Materials/MaterialParameterCollectionInstance.h" 
#include "Engine/PostProcessVolume.h"

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

	paramInstance = GetWorld()->GetParameterCollectionInstance(outlineParams);
}

void ARebuildSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(rebuildSwitchPlayer && switchActivated == false)
	{
		if (rebuildSwitchPlayer->GetActorLocation().Equals(GetActorLocation()))
		{
			//rebuildSwitchPlayer->bFadeOutRebuild = true;
			bRebuildPulseEffect = true;

			rebuildTimer += FApp::GetDeltaTime();
			rebuildSwitchPlayer->canMove = false;

			RebuildAll();

			//Rebuild Pulse effect
			if (bRebuildPulseEffect && rebuildPulseEffectTimer == 0.f)
			{
				bRebuildPulseEffect = false;
				rebuildPulseEffectTimer += FApp::GetDeltaTime();
				postProcessMain->Settings.AddBlendable(postProcessOutline, 1.0f);
			}

			if (rebuildPulseEffectTimer < 2.0f)
			{
				rebuildPulseEffectValue += FApp::GetDeltaTime() * 500.f;
				paramInstance->SetScalarParameterValue(TEXT("Radius"), rebuildPulseEffectValue);
				rebuildPulseEffectTimer += FApp::GetDeltaTime();
			}
			else if (rebuildPulseEffectTimer > 2.0f)
			{
				rebuildPulseEffectValue = 0.f;
				paramInstance->SetScalarParameterValue(TEXT("Radius"), rebuildPulseEffectValue);

				postProcessMain->Settings.RemoveBlendable(postProcessOutline);
				rebuildPulseEffectTimer = 0.f;
			}

			if (rebuildTimer > 2.0f) //half of what is being used in Mecha.cpp
			{
				rebuildSwitchPlayer->canMove = true;
				switchActivated = true;

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
}
