// Fill out your copyright notice in the Description page of Project Settings.

#include "RotatingBoss.h"
#include "Engine/World.h"
#include "LevelAudio.h"
#include "GlobalTags.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DestructibleComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Mecha.h"
#include "Materials/MaterialParameterCollectionInstance.h" 
#include "Engine/PostProcessVolume.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"


ARotatingBoss::ARotatingBoss()
{
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add(Tags::CantMove);
}

void ARotatingBoss::BeginPlay()
{
	Super::BeginPlay();
	
	minionSpawningBox = FindComponentByClass<UBoxComponent>();
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

		FVector spawnPoint = UKismetMathLibrary::RandomPointInBoundingBox(minionSpawningBox->GetComponentLocation(), minionSpawningBox->Bounds.BoxExtent);
		/*FQuat spawnQuat;
		FCollisionShape spawnBox;
		spawnBox.SetBox(FVector(50.f));
		TArray<FOverlapResult> spawnOverlaps;
		GetWorld()->OverlapMultiByChannel(spawnOverlaps, spawnPoint, spawnQuat, ECC_WorldStatic, spawnBox);

		while(spawnOverlaps.Num() != 0)
		{
			spawnOverlaps.Empty();
			spawnPoint = UKismetMathLibrary::RandomPointInBoundingBox(minionSpawningBox->GetComponentLocation(), minionSpawningBox->Bounds.BoxExtent);
			GetWorld()->OverlapMultiByChannel(spawnOverlaps, spawnPoint, spawnQuat, ECC_WorldStatic, spawnBox);
		}*/

		switch (bossStage)
		{
		case 1:
			rotatingComponent->RotationRate.Yaw = FMath::RandRange(-180.f, 180.f);
			rotatingComponent->RotationRate.Roll = FMath::RandRange(-180.f, 180.f);
			rotatingComponent->RotationRate.Pitch = FMath::RandRange(-180.f, 180.f);

			GetWorld()->SpawnActor<AActor>(minionClass, spawnPoint, FRotator());

			break;
		case 2:
			rotatingComponent->RotationRate.Yaw = FMath::RandRange(-360.f, 360.f);
			rotatingComponent->RotationRate.Roll = FMath::RandRange(-360.f, 360.f);
			rotatingComponent->RotationRate.Pitch = FMath::RandRange(-360.f, 360.f);
			break;
		case 3:
			rotatingComponent->RotationRate.Yaw = FMath::RandRange(-720.f, 720.f);
			rotatingComponent->RotationRate.Roll = FMath::RandRange(-720.f, 720.f);
			rotatingComponent->RotationRate.Pitch = FMath::RandRange(-720.f, 720.f);
			break;
		case 4:
			rotatingComponent->RotationRate.Yaw = 0.f;
			rotatingComponent->RotationRate.Roll = 0.f;
			rotatingComponent->RotationRate.Pitch = 0.f;
			break;
		}
	}

	//Pulse effect
	if (bossStage < 3)
	{
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

				bCanBeHit = true;
			}
		}
	}
	else if (bossStage == 3)
	{
		bCanBeHit = true;
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

	if (bossStage < 3)
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
				}
			}

			attachActors[i]->SetLifeSpan(timerOnSpawn / 2.f);
		}

		FTimerHandle timerHandle;
		GetWorldTimerManager().SetTimer(timerHandle, this, &ARotatingBoss::SpawnNewBody, timerOnSpawn, false);
	}
	else if (bossStage >= 3)
	{
		bossStage++;
		spinTimer = spinTimerMax;
		FindComponentByClass<UStaticMeshComponent>()->SetSimulatePhysics(true);
		bCanBeHit = false;
		FTimerHandle timerHandle;
		GetWorldTimerManager().SetTimer(timerHandle, this, &ARotatingBoss::BossDeath, 10.0f, false);

		for (int i = 0; i < attachActors.Num(); i++)
		{
			TArray<AActor*> childActorsOnBody;
			attachActors[i]->GetAllChildActors(childActorsOnBody);
			for (int j = 0; j < childActorsOnBody.Num(); j++)
			{
				UDestructibleComponent* dc = childActorsOnBody[j]->FindComponentByClass<UDestructibleComponent>();
				if (dc)
				{
					dc->SetSimulatePhysics(true);
					dc->GetOwner()->SetLifeSpan(10.f);
				}
			}

			UGameplayStatics::PlaySoundAtLocation(GetWorld(), soundHit, GetActorLocation(), 1.0f, 5.0f);
			
			TArray<AActor*> levelAudio;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALevelAudio::StaticClass(), levelAudio);
			for (AActor* audio : levelAudio)
			{
				audio->FindComponentByClass<UAudioComponent>()->FadeOut(5.0f, 0.f);
			}

			attachActors[i]->SetLifeSpan(10.0f);
		}
	}

	//rotatingComponent->PivotTranslation = FVector(FMath::RandRange(-50.f, 50.f));
}

void ARotatingBoss::SpawnNewBody()
{
	//Spawn new body
	AActor* newBody = GetWorld()->SpawnActor<AActor>(bodyToSpawn, GetActorLocation(), GetActorRotation());
	newBody->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));

	AMecha* player = Cast<AMecha>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	player->instancedRebuildManager->ClearAll();

	bossStage++;

	bRebuildPulseEffect = true;
}

void ARotatingBoss::BossDeath()
{
	AMecha* player = Cast<AMecha>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	player->instancedRebuildManager->ClearAll();
	
	//Open level door
	doorToOpen->Destroy();

	Destroy();
}
