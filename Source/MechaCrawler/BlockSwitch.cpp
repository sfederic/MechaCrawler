// Fill out your copyright notice in the Description page of Project Settings.

#include "BlockSwitch.h"
#include "GameFramework/Pawn.h"
#include "Pushable.h"
#include "Activate.h"
#include "Kismet/GameplayStatics.h"
#include "Components/MeshComponent.h"
#include "DestructibleComponent.h"

ABlockSwitch::ABlockSwitch()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABlockSwitch::BeginPlay()
{
	Super::BeginPlay();
	
	mesh = FindComponentByClass<UMeshComponent>();
	check(mesh);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APushable::StaticClass(), pushableActors);

	player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void ABlockSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//TODO: fix this shit, this is awful
	for (int i = 0; i < pushableActors.Num(); i++)
	{
		if (pushableActors[i]->GetActorLocation().Equals(this->GetActorLocation()))
		{
			mesh->SetMaterial(0, activatedMat);
			pushableActors.Empty();
			ActivateConnection();
			return;
		}
	}

	if (player->GetActorLocation().Equals(this->GetActorLocation()))
	{
		mesh->SetMaterial(0, activatedMat);
		return;
	}
	
	mesh->SetMaterial(0, deactivatedMat);
}

void ABlockSwitch::ActivateConnection()
{
	if (connectedActor)
	{
		IActivate* connect = Cast<IActivate>(connectedActor);
		if (connect)
		{
			connect->Use();
		}

		if (connectedActor->IsA<ADestructibleActor>())
		{
			UDestructibleComponent* dc = connectedActor->FindComponentByClass<UDestructibleComponent>();
			if (dc)
			{
				dc->ApplyDamage(10000.f, connectedActor->GetActorLocation(), connectedActor->GetActorForwardVector(), 1000.f);
			}
		}
	}
}
