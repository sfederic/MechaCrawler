// Fill out your copyright notice in the Description page of Project Settings.

#include "ScanData.h"

UScanData::UScanData()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UScanData::BeginPlay()
{
	Super::BeginPlay();
}

void UScanData::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
