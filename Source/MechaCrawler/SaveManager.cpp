// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveManager.h"
#include "Kismet/GameplayStatics.h"
#include "LevelSave.h"

ASaveManager::ASaveManager()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ASaveManager::BeginPlay()
{
	Super::BeginPlay();

}

void ASaveManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

