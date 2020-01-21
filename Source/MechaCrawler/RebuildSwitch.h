// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RebuildSwitch.generated.h"

//TODO: Make sure that player can't go through re-spawned actors. Set a timer for this

UCLASS()
class MECHACRAWLER_API ARebuildSwitch : public AActor
{
	GENERATED_BODY()
	
public:	
	ARebuildSwitch();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	void RebuildAll();

	float rebuildTimer;

	bool switchActivated = false;

	class AMecha* rebuildSwitchPlayer;
	class APawn* rebuildSwitchPlayerPawn;

	UPROPERTY(EditAnywhere)
	class UMaterialParameterCollection* outlineParams;

	UPROPERTY(EditAnywhere)
	class APostProcessVolume* postProcessMain;

	UPROPERTY(EditAnywhere)
	UMaterialInstance* postProcessOutline;

	class UMaterialParameterCollectionInstance* paramInstance;

	bool bRebuildPulseEffect = false;
	float rebuildPulseEffectTimer = 0.f;
	float rebuildPulseEffectValue = 0.f;
};
