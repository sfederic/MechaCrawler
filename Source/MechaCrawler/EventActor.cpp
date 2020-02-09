// Fill out your copyright notice in the Description page of Project Settings.

#include "EventActor.h"
#include "Components/BoxComponent.h"

AEventActor::AEventActor()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AEventActor::BeginPlay()
{
	Super::BeginPlay();

	if (bIsActivated)
	{
		FindComponentByClass<UBoxComponent>()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	else if (!bIsActivated)
	{
		FindComponentByClass<UBoxComponent>()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AEventActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEventActor::Activate()
{
	bIsActivated = true;
	FindComponentByClass<UBoxComponent>()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

