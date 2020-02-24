// Fill out your copyright notice in the Description page of Project Settings.

#include "Turret.h"
#include "GlobalTags.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h" 
#include "Engine/World.h"
#include "Misc/App.h"
#include "Particles/ParticleSystemComponent.h"
#include "DestructibleComponent.h"
#include "Mecha.h"

ATurret::ATurret()
{
	PrimaryActorTick.bCanEverTick = true;
	shootParams.AddIgnoredActor(this);
}

void ATurret::BeginPlay()
{
	Super::BeginPlay();

	target = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	attackTimer = FMath::RandRange(0.f, attackInterval);
}

void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	attackTimer += FApp::GetDeltaTime();

	if (attackTimer > attackInterval)
	{
		UParticleSystemComponent* laserParticle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), laserParticleTemplate, GetActorLocation(), FRotator());
		laserParticle->SetBeamSourcePoint(0, GetActorLocation(), 0);

		FHitResult shootHit;
		if (GetWorld()->LineTraceSingleByChannel(shootHit, GetActorLocation(), target->GetActorLocation() + FVector(FMath::FRandRange(-100.f, 100.f)), ECC_WorldStatic, shootParams))
		{
			laserParticle->SetBeamTargetPoint(0, shootHit.ImpactPoint, 0);
			if (shootHit.GetActor())
			{
				UDestructibleComponent* dc = shootHit.GetActor()->FindComponentByClass<UDestructibleComponent>();
				if (dc)
				{
					dc->ApplyDamage(1000.f, shootHit.GetActor()->GetActorLocation(), shootHit.ImpactNormal, 7500.f);

					AMecha* player = Cast<AMecha>(target);

					if (shootHit.GetActor())
					{
						player->AddDestructibleToRebuildManager(shootHit.GetActor());
					}

					//Old method of AddDestructibleToRebuildManager(). Keep just incase...
					/*ADestructibleActor* rebuildActor = Cast<ADestructibleActor>(shootHit.GetActor());

					if (player->instancedRebuildManager && dc->GetOwner()->Tags.Contains(Tags::Destroy) == false)
					{
						dc->GetOwner()->Tags.Add(Tags::Destroy);

						if (rebuildActor && rebuildActor->IsA<ADestructibleActor>())
						{
							if (rebuildActor->Tags.Contains(Tags::DontRebuild) == false)
							{
								player->instancedRebuildManager->rebuildActors.Add(rebuildActor);
								player->instancedRebuildManager->rebuildTimers.Add(0.f);
								player->instancedRebuildManager->rebuildActorFadeMaterials.Add(rebuildActor->FindComponentByClass<UDestructibleComponent>()->GetMaterial(0));
							}
							else
							{
								rebuildActor->SetLifeSpan(2.0f);
							}
						}
					}*/
				}
			}
		}
		else
		{
			laserParticle->SetBeamTargetPoint(0, GetActorLocation() + (GetActorForwardVector() * 1000.f), 0);
		}

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), shootSound, shootHit.ImpactPoint);

		attackTimer = 0.f;
	}

	if (target)
	{
		SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), target->GetActorLocation()));
	}
}
