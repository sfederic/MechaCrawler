// Fill out your copyright notice in the Description page of Project Settings.


#include "EventActor.h"

AEventActor::AEventActor()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AEventActor::BeginPlay()
{
	Super::BeginPlay();

	box->OnComponentBeginOverlap.AddDynamic(this, &AEventActor::Activate);

}

void AEventActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEventActor::Activate()
{
	bIsActivated = true;
}

