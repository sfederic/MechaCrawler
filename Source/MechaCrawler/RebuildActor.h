// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RebuildActor.generated.h"

UCLASS()
class MECHACRAWLER_API ARebuildActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ARebuildActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	float rebuildTimer = 0.f;
	bool isRebuilding = false;
};
