// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleItem.h"

APuzzleItem::APuzzleItem()
{
	PrimaryActorTick.bCanEverTick = true;

}

void APuzzleItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void APuzzleItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APuzzleItem::Rebuild()
{
	bActivated = false;
}

void APuzzleItem::Use()
{

}

