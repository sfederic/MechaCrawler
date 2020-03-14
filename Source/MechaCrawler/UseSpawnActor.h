// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Activate.h"
#include "UseSpawnActor.generated.h"

//Class that gets spawned on Use(). Good for doors that close behind player. Can work in on or off state

UCLASS()
class MECHACRAWLER_API AUseSpawnActor : public AActor, public IActivate
{
	GENERATED_BODY()
	
public:	
	AUseSpawnActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void Use() override;

	UPROPERTY(EditAnywhere)
	bool bStartsActive;
};
