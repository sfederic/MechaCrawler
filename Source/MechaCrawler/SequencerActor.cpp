// Fill out your copyright notice in the Description page of Project Settings.


#include "SequencerActor.h"

ASequencerActor::ASequencerActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ASequencerActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (bPlayOnStart)
	{
		shot->PlaybackSettings.bAutoPlay = true;
	}
}

void ASequencerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

