// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Rebuild.h"
#include "Activate.h"
#include "PuzzleItem.generated.h"


//Make the other puzzle items inherit from this sothey can all have their own bullshit logic

UCLASS()
class MECHACRAWLER_API APuzzleItem : public AActor, public IRebuild, public IActivate
{
	GENERATED_BODY()
	
public:	
	APuzzleItem();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void Rebuild() override;
	virtual void Use() override;

	bool bActivated; //This is how the puzzle manager is going to do this
};
