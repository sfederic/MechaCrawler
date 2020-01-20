// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestructibleActor.h"
#include "GridActor.h"
#include "DestructibleComponent.h"
#include "IceBlock.h"
#include "Pushable.h"
#include "RebuildManager.generated.h"

UCLASS()
class MECHACRAWLER_API ARebuildManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ARebuildManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	void RebuildTimers();
	void RebuildPushables();
	void RebuildDoors();

	UPROPERTY(EditAnywhere)
	bool debugActors;

	class AMecha* player;

	UPROPERTY()
	TArray<ADestructibleActor*> rebuildActors;

	UPROPERTY()
	TArray<float> rebuildTimers;

	UPROPERTY()
	TArray<AActor*> movingGridActors;

	UPROPERTY()
	TArray<AActor*> pushableActors;
};
