// Fill out your copyright notice in the Description page of Project Settings.


#include "SmallRotatingEnemy.h"
#include "GlobalTags.h"
#include "DestructibleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "GameFramework/RotatingMovementComponent.h"

ASmallRotatingEnemy::ASmallRotatingEnemy()
{
	this->PrimaryActorTick.bCanEverTick = true;
}

void ASmallRotatingEnemy::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add(Tags::CantMove); //Just remember that you can't add tags in constructor
	Tags.Add(Tags::Pickup);
	Tags.Add(Tags::Useable);
}

void ASmallRotatingEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASmallRotatingEnemy::ActivateHitEffect()
{
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
}
