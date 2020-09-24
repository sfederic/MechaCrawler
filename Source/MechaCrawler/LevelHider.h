// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelHider.generated.h"

//Class for attaching actors to and hiding/revealing based on memory orbs to reveal new parts of level.
//Opting for this approach instead of level streaming, need to see the level spawn

UCLASS()
class MECHACRAWLER_API ALevelHider : public AActor
{
	GENERATED_BODY()
	
public:	
	ALevelHider();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	bool bRevealLevel = false;
};
