// Fill out your copyright notice in the Description page of Project Settings.

#include "BlockSwitch.h"
#include "GameFramework/Pawn.h"
#include "Pushable.h"
#include "Activate.h"
#include "Kismet/GameplayStatics.h"
#include "Components/MeshComponent.h"

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

	for (int i = 0; i < pushableActors.Num(); i++)
	{
		if (pushableActors[i]->GetActorLocation().Equals(this->GetActorLocation()))
		{
			mesh->SetMaterial(0, activatedMat);
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
		connect->Use();
	}
}
