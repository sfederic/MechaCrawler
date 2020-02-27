// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelAudio.h"
#include "Runtime/Engine/Public/TimerManager.h"


ALevelAudio::ALevelAudio()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ALevelAudio::BeginPlay()
{
	Super::BeginPlay();

	song = FindComponentByClass<UAudioComponent>();
	if (song)
	{
		song->Play();
	}
}

void ALevelAudio::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelAudio::FadeOutAudioAndStopOnTimer()
{
	song->FadeOut(1.5f, 0.f);
	FTimerHandle handle;
	//GetWorldTimerManager().SetTimer(handle, this, &ALevelAudio::StopAudio, 3.0f);
}

void ALevelAudio::StopAudio()
{
	song->Stop();
}
