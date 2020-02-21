// Fill out your copyright notice in the Description page of Project Settings.

#include "DestructibleActivate.h"
#include "DestructibleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GlobalTags.h"
#include "RebuildManager.h"
#include "Mecha.h"

ADestructibleActivate::ADestructibleActivate()
{
	this->PrimaryActorTick.bCanEverTick = true;
}

void ADestructibleActivate::BeginPlay()
{
	Super::BeginPlay();

}

void ADestructibleActivate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CheckAllSwitches())
	{
		if (Tags.Contains(Tags::Destroy) == false)
		{
			FindComponentByClass<UDestructibleComponent>()->ApplyDamage(1000.f, this->GetActorLocation(), GetActorForwardVector(), 1000.f);

			Tags.Add(Tags::Destroy);

			if (rebuildManager)
			{
				rebuildManager->rebuildActorsActivate.Add(this);
				rebuildManager->rebuildActorActivateFadeMaterials.Add(FindComponentByClass<UDestructibleComponent>()->GetMaterial(0));
				rebuildManager->rebuildActivateTimers.Add(0.f);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Rebuild maanager not set for %s\n"), *GetNameSafe(this));
			}
		}
	}
}

void ADestructibleActivate::Rebuild()
{
	switches.Empty();

	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADestructibleSwitch::StaticClass(), actors);

	for (int i = 0; i < actors.Num(); i++)
	{
		switches.Add(Cast<ADestructibleSwitch>(actors[i]));
	}

	APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	AMecha* player = Cast<AMecha>(playerPawn);
	this->rebuildManager = player->instancedRebuildManager;	
}


bool ADestructibleActivate::CheckAllSwitches()
{
	for (int i = 0; i < switches.Num(); i++)
	{
		if (switches[i]->bDestroyed == false)
		{
			return false;
		}
	}

	if (switches.Num() == 0)
	{
		return false;
	}

	return true;
}
