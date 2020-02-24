// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"
#include "LevelAudio.generated.h"

//Main song audio per level. Here because need a way to turn of all songs on dialogueboxes with GetAllActors()

UCLASS()
class MECHACRAWLER_API ALevelAudio : public AActor
{
	GENERATED_BODY()
	
public:	
	ALevelAudio();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	//void OnFinishAudio() //remember that there's a delegate in UAudioComponent by OnAudioFinished()
	void FadeOutAudioAndStopOnTimer();
	void StopAudio();

	UPROPERTY(EditAnywhere)
	UAudioComponent* song;
};
