// Fill out your copyright notice in the Description page of Project Settings.


#include "MechaCrawlerGameModeBase.h"
#include "PlayerHUD.h"

void AMechaCrawlerGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	HUDClass = APlayerHUD::StaticClass();
}

void AMechaCrawlerGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
