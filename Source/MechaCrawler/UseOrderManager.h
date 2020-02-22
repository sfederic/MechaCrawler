// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleItem.h"
#include "Rebuild.h"
#include "UseOrderManager.generated.h"

//Class for managing a Zelda-esque puzzle of "do thing in an specific order"

UCLASS()
class MECHACRAWLER_API AUseOrderManager : public AActor, public IRebuild
{
	GENERATED_BODY()
	
public:	
	AUseOrderManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void Rebuild() override;

	UPROPERTY(EditAnywhere)
	AActor* useActor; //Linked actor the manager will call on Activation

	UPROPERTY(VisibleAnywhere)
	TArray<int> activatedItems;

	UPROPERTY(VisibleAnywhere)
	int activateCounter;

	UPROPERTY(EditAnywhere)
	TArray<int> itemActivateOrder; //The puzzleItems activation order
};
