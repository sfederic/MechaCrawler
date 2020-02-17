// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelSequenceActor.h"
#include "SequencerActor.generated.h"

//This exists because of OnStop(). Can't be bothered making Events in BP to call bools on Mecha to stop from moving

UCLASS()
class MECHACRAWLER_API ASequencerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ASequencerActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	ALevelSequenceActor* shot;

	UPROPERTY(EditAnywhere)
	bool bPlayOnStart;
};
