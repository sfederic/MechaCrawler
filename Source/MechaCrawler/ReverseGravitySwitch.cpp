// Fill out your copyright notice in the Description page of Project Settings.

#include "ReverseGravitySwitch.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Pushable.h"

AReverseGravitySwitch::AReverseGravitySwitch()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AReverseGravitySwitch::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APushable::StaticClass(), actors);

	pushActors.Reserve(actors.Num());

	for (int i = 0; i < actors.Num(); i++)
	{
		pushActors.Add(Cast<APushable>(actors[i]));
	}
}

void AReverseGravitySwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AReverseGravitySwitch::Use()
{
	for (int i = 0; i < pushActors.Num(); i++)
	{
		pushActors[i]->gravityVector = -pushActors[i]->gravityVector; //Reverse the vector	
	}
}
