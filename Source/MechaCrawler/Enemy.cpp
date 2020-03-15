// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "DestructibleComponent.h"
#include "Components/WidgetComponent.h"

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

void AEnemy::DropLoot()
{

}

void AEnemy::ActivateHitEffect()
{

}
