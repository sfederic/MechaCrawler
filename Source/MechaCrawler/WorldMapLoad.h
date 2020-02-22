// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldMapLoad.generated.h"

//Not the same as LoadPoint, need this to transfer player locations from map to world map

UCLASS()
class MECHACRAWLER_API AWorldMapLoad : public AActor
{
	GENERATED_BODY()
	
public:	
	AWorldMapLoad();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
