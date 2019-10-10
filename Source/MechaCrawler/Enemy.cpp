// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

const int movesPerTurn = 10;

FVector movePositions[movesPerTurn][movesPerTurn];

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

}


void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

