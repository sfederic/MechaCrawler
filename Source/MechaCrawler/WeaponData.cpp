// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponData.h"

UWeaponData::UWeaponData()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UWeaponData::BeginPlay()
{
	Super::BeginPlay();
}

void UWeaponData::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
