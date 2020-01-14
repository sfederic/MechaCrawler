// Fill out your copyright notice in the Description page of Project Settings.

#include "AirEnemy.h"

AAirEnemy::AAirEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAirEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AAirEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

