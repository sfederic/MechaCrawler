// Fill out your copyright notice in the Description page of Project Settings.

#include "RebuildSwitch.h"
#include "Kismet/GameplayStatics.h"
#include "Mecha.h"
#include "RebuildManager.h"
#include "IceComponent.h"
#include "Materials/MaterialParameterCollectionInstance.h" 
#include "Engine/PostProcessVolume.h"
#include "Components/BoxComponent.h"

ARebuildSwitch::ARebuildSwitch()
{
	PrimaryActorTick.bCanEverTick = true;
	rebuildTimer = 0.0f;
	pulseSpeed = 300.f;
	pulseTimerMax = 3.0f;
}

void ARebuildSwitch::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<AActor*> postProcessVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APostProcessVolume::StaticClass(), postProcessVolumes);
	for (int i = 0; i < postProcessVolumes.Num(); i++)
	{
		postProcessMain = Cast<APostProcessVolume>(postProcessVolumes[i]);
		break;
	}

	UBoxComponent* box = FindComponentByClass<UBoxComponent>();
	box->OnComponentBeginOverlap.AddDynamic(this, &ARebuildSwitch::OnPlayerOverlapBegin);

	rebuildSwitchPlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	rebuildSwitchPlayer = Cast<AMecha>(rebuildSwitchPlayerPawn);

	paramInstance = GetWorld()->GetParameterCollectionInstance(outlineParams);
	paramInstance->SetVectorParameterValue("StartingPosition", GetActorLocation());
}

void ARebuildSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (rebuildSwitchPlayer && switchActivated == true)
	{
		//if (rebuildSwitchPlayer->GetActorLocation().Equals(GetActorLocation()))
		{
			rebuildTimer += FApp::GetDeltaTime();

			if (rebuildTimer > 0.5f) //Magic number here because don't want rebuild to trigger when just walking through bounds, needs a while to warm up
			{
				//rebuildSwitchPlayer->bFadeOutRebuild = true;
				bRebuildPulseEffect = true;

				rebuildSwitchPlayer->canMove = false;

				RebuildAll();

				//Rebuild Pulse effect
				if (bRebuildPulseEffect && rebuildPulseEffectTimer == 0.f)
				{
					bRebuildPulseEffect = false;
					rebuildPulseEffectTimer += FApp::GetDeltaTime();

					if (postProcessMain)
					{
						postProcessMain->Settings.AddBlendable(postProcessOutline, 1.0f);
					}
				}

				if (rebuildPulseEffectTimer < pulseTimerMax)
				{
					rebuildPulseEffectValue += FApp::GetDeltaTime() * pulseSpeed;
					paramInstance->SetScalarParameterValue(TEXT("Radius"), rebuildPulseEffectValue);
					rebuildPulseEffectTimer += FApp::GetDeltaTime();
				} 
				else if (rebuildPulseEffectTimer > pulseTimerMax)
				{
					rebuildPulseEffectValue = 0.f;
					paramInstance->SetScalarParameterValue(TEXT("Radius"), rebuildPulseEffectValue);

					if (postProcessMain)
					{
						postProcessMain->Settings.RemoveBlendable(postProcessOutline);
					}

					rebuildPulseEffectTimer = 0.f;
				}

				if (rebuildTimer > pulseTimerMax && rebuildPulseEffectTimer == 0.f) //this is awful using the two timers together
				{
					rebuildSwitchPlayer->canMove = true;
					switchActivated = true;

					rebuildTimer = 0.f;

					if (switchActivated == true)
					{
						switchActivated = false;
					}
				}
			}
		}
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

void ARebuildSwitch::OnPlayerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AMecha>())
	{
		if (switchActivated == false)
		{
			switchActivated = true;
		}
	}
}
