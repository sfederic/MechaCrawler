// Fill out your copyright notice in the Description page of Project Settings.


#include "SmallRotatingEnemy.h"
#include "GlobalTags.h"
#include "DestructibleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

ASmallRotatingEnemy::ASmallRotatingEnemy()
{
	this->PrimaryActorTick.bCanEverTick = true;
	shootParams.AddIgnoredActor(this);
	shootTimer = FMath::RandRange(0.f, shootTimerMax);
}

void ASmallRotatingEnemy::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add(Tags::CantMove); //Just remember that you can't add tags in constructor
	Tags.Add(Tags::Pickup);
	Tags.Add(Tags::Useable);

	SetActorScale3D(FVector(0.f)); //Want to try an spawn effect (scale up on spawn)

	rotatingComponent = FindComponentByClass<URotatingMovementComponent>();
}

void ASmallRotatingEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetActorScale3D().X >= 1.0f)
	{
		shootTimer += FApp::GetDeltaTime();
		bCanBeHit = true;
	}
	else
	{
		SetActorScale3D(GetActorScale3D() + FVector(0.01f));
		bCanBeHit = false;
	}

	rotatingComponent->RotationRate.Yaw += FApp::GetDeltaTime() * 250.f;

	if (shootTimer > shootTimerMax && bIsAlive)
	{
		APawn* player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (GetWorld()->LineTraceSingleByChannel(shootHit, GetActorLocation(), player->GetActorLocation(), ECC_WorldStatic, shootParams))
		{
			UParticleSystemComponent* shootParticle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), shootParticleTemplate, GetActorLocation(), FRotator());
			shootParticle->SetBeamSourcePoint(0, GetActorLocation(), 0);
			shootParticle->SetBeamTargetPoint(0, shootHit.ImpactPoint, 0);

			UGameplayStatics::PlaySoundAtLocation(GetWorld(), soundShoot, GetActorLocation(), 1.0f, 1.0f);

			shootTimer = 0.f;
		}

		rotatingComponent->RotationRate.Yaw = 0.f;
	}
}

void ASmallRotatingEnemy::ActivateHitEffect()
{
	bIsAlive = false;

	UDestructibleComponent* dc = this->FindComponentByClass<UDestructibleComponent>();
	if (dc)
	{
		dc->ApplyDamage(1000.f, GetActorLocation(), GetActorForwardVector(), 1000.f);
		FTimerHandle handle;
		GetWorldTimerManager().SetTimer(handle, this, &ASmallRotatingEnemy::DestroyDestructible, 2.f, false);
	}
	
	//FindComponentByClass<URotatingMovementComponent>()->DestroyComponent();
	//FindComponentByClass<UStaticMeshComponent>()->SetSimulatePhysics(true);
	bCanBeHit = false;
}

void ASmallRotatingEnemy::DestroyDestructible()
{
	FindComponentByClass<UDestructibleComponent>()->DestroyComponent();
	Destroy();
}
