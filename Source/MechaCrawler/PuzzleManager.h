// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleItem.h"
#include "LevelSequenceActor.h"
#include "PuzzleManager.generated.h"

//I guess it's a class for managing puzzle peices that will use Use() and Rebuild() interfaces

UCLASS()
class MECHACRAWLER_API APuzzleManager : public AActor
{
	GENERATED_BODY()
	
public:	
	APuzzleManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnCinematicStop();

	UPROPERTY(EditAnywhere)
	AActor* useActor;

	UPROPERTY(EditAnywhere)
	ULevelSequence* puzzleCompleteShot;

	ALevelSequenceActor* sequenceActor;

	UPROPERTY(EditAnywhere)
	TArray<APuzzleItem*> puzzleItems;

	UPROPERTY(VisibleAnywhere)
	bool bActivated = false;
};
