// Fill out your copyright notice in the Description page of Project Settings.

#include "UseVolume.h"
#include "Components/BoxComponent.h"
#include "Activate.h"
#include "DestructibleComponent.h"

AUseVolume::AUseVolume()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AUseVolume::BeginPlay()
{
	Super::BeginPlay();
	
	volume = FindComponentByClass<UBoxComponent>();
	volume->OnComponentBeginOverlap.AddDynamic(this, &AUseVolume::OnActorOverlap);
}

void AUseVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUseVolume::ActivateConnections()
{
	for (int i = 0; i < connectedActors.Num(); i++)
	{
		IActivate* activate = Cast<IActivate>(connectedActors[i]);
		if (activate)
		{
			activate->Use();
		}
		else if (connectedActors[i]->FindComponentByClass<UDestructibleComponent>())
		{
			connectedActors[i]->FindComponentByClass<UDestructibleComponent>()->ApplyDamage(100000.f, connectedActors[i]->GetActorLocation(), -FVector::UpVector, 10000.f);
			connectedActors[i]->SetLifeSpan(2.0f); //Instead of putting onto rebuilable stack

		}
	}
}

void AUseVolume::OnActorOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ActivateConnections();
}
