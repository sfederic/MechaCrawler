// Fill out your copyright notice in the Description page of Project Settings.

#include "SwitchReceiver.h"
#include "Engine/World.h"
#include "BlockSwitch.h"
#include "Kismet/GameplayStatics.h"

ASwitchReceiver::ASwitchReceiver()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ASwitchReceiver::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABlockSwitch::StaticClass(), actors);
	switchActors.Reserve(actors.Num());
	for (int i = 0; i < actors.Num(); i++)
	{
		switchActors.Add(Cast<ABlockSwitch>(actors[i]));
	}
}

void ASwitchReceiver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	int allActivated = 0; //Making it so that if this is > 0, it'll break
	for (ABlockSwitch* block : switchActors)
	{
		if (block->activated == false)
		{
			allActivated++;
		}
	}

	if (allActivated == 0)
	{
		//...
	}
}

