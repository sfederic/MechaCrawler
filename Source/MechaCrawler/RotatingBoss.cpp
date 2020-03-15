// Fill out your copyright notice in the Description page of Project Settings.

#include "RotatingBoss.h"
#include "GlobalTags.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DestructibleComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Mecha.h"
#include "Materials/MaterialParameterCollectionInstance.h" 
#include "Engine/PostProcessVolume.h"


ARotatingBoss::ARotatingBoss()
{
	PrimaryActorTick.bCanEverTick = true;
	
}

void ARotatingBoss::BeginPlay()
{
	Super::BeginPlay();
	
	rotatingComponent = FindComponentByClass<URotatingMovementComponent>();
	paramInstance = GetWorld()->GetParameterCollectionInstance(outlineParams);
	paramInstance->SetVectorParameterValue("StartingPosition", GetActorLocation());
}

void ARotatingBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Random spinning movement
	spinTimer += FApp::GetDeltaTime();
	if (spinTimer > spinTimerMax)
	{
		spinTimer = 0.f;
		rotatingComponent->RotationRate.Yaw = FMath::RandRange(-180.f, 180.f);
		rotatingComponent->RotationRate.Roll = FMath::RandRange(-180.f, 180.f);
		rotatingComponent->RotationRate.Pitch = FMath::RandRange(-180.f, 180.f);
	}

	//Pulse effect
	if (bRebuildPulseEffect && rebuildPulseEffectTimer == 0.f)
	{
		bRebuildPulseEffect = false;
		rebuildPulseEffectTimer += FApp::GetDeltaTime();

		if (postProcessMain)
		{
			postProcessMain->Settings.AddBlendable(postProcessOutline, 1.0f);
		}
	}

	if (rebuildPulseEffectTimer > 0.0f)
	{
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
	}
}

void ARotatingBoss::ActivateHitEffect()
{
	bCanBeHit = false;
	spinTimer = 0.f;

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), soundHit, GetActorLocation(), 3.f, 3.f);
	rotatingComponent->RotationRate = FRotator(0.f);

	//TArray<UActorComponent*> childActors; 
	//childActors = GetComponentsByClass(UChildActorComponent::StaticClass());

	TArray<AActor*> attachActors;
	this->GetAttachedActors(attachActors);

	if (attachActors.Num() > 0)
	{
		for (int i = 0; i < attachActors.Num(); i++)
		{
			TArray<AActor*> childActorsOnBody;
			attachActors[i]->GetAllChildActors(childActorsOnBody);
			for (int j = 0; j < childActorsOnBody.Num(); j++)
			{
				UDestructibleComponent* dc = childActorsOnBody[j]->FindComponentByClass<UDestructibleComponent>();
				if (dc)
				{
					dc->ApplyDamage(1000.f, childActorsOnBody[j]->GetActorLocation(), childActorsOnBody[j]->GetActorForwardVector(), 1000.f);
					dc->GetOwner()->SetLifeSpan(timerOnSpawn / 2.f);
				}
			}
		}
	}
	/*else
	{
		for (int i = 0; i < childActors.Num(); i++)
		{
			UChildActorComponent* child = Cast<UChildActorComponent>(childActors[i]);
			if (child)
			{
				AActor* actor = child->GetChildActor();
				TArray<AActor*> childActorsOnBody;
				actor->GetAllChildActors(childActorsOnBody);
				for (int j = 0; j < childActorsOnBody.Num(); j++)
				{
					UDestructibleComponent* dc = childActorsOnBody[j]->FindComponentByClass<UDestructibleComponent>();
					if (dc)
					{
						dc->ApplyDamage(1000.f, childActorsOnBody[j]->GetActorLocation(), childActorsOnBody[j]->GetActorForwardVector(), 10000.f);
						childActorsOnBody[i]->SetLifeSpan(timerOnSpawn / 2.f);
					}
				}
			}
		}
	}*/

	FTimerHandle timerHandle;
	GetWorldTimerManager().SetTimer(timerHandle, this, &ARotatingBoss::SpawnNewBody, timerOnSpawn, false);

	//rotatingComponent->PivotTranslation = FVector(FMath::RandRange(-50.f, 50.f));
}

void ARotatingBoss::SpawnNewBody()
{
	//Spawn new body
	AActor* newBody = GetWorld()->SpawnActor<AActor>(bodyToSpawn, GetActorLocation(), FRotator(0.f));
	newBody->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false));
	//newBody->SetActorLocation(this->GetActorLocation());

	AMecha* player = Cast<AMecha>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	player->instancedRebuildManager->ClearAll();

	bCanBeHit = true;

	bRebuildPulseEffect = true;
}
