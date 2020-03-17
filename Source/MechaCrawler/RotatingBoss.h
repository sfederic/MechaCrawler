// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "RotatingBoss.generated.h"

//For the boss that Rotates and hides its weakpoint with destructible objects

UCLASS()
class MECHACRAWLER_API ARotatingBoss : public AEnemy
{
	GENERATED_BODY()
	
public:	
	ARotatingBoss();

protected:
	virtual void BeginPlay() override;

public:	
	class URotatingMovementComponent* rotatingComponent;

	virtual void Tick(float DeltaTime) override;
	virtual void ActivateHitEffect() override;
	void SpawnNewBody();
	void BossDeath();

	class UBoxComponent* minionSpawningBox;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> bodyToSpawn;

	UPROPERTY(EditAnywhere)
	USoundBase* soundHit;

	UPROPERTY(EditAnywhere)
	class APostProcessVolume* postProcessMain;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* postProcessOutline;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> minionClass;

	class UMaterialParameterCollectionInstance* paramInstance;

	UPROPERTY(EditAnywhere)
	class UMaterialParameterCollection* outlineParams;

	float spinTimer = 5.f; //Keep it same as spinTimerMax so actor doesn't start not spinning;
	float spinTimerMax = 5.f;
	float timerOnSpawn = 2.0f;
	float rebuildPulseEffectTimer = 0.f;
	float pulseTimerMax = 3.0f;
	float rebuildPulseEffectValue;

	UPROPERTY(EditAnywhere)
	float pulseSpeed;

	UPROPERTY(EditAnywhere)
	int bossStage; //Stages of the boss fight

	UPROPERTY(EditAnywhere)
	AActor* doorToOpen;

	bool bNextRotate;
	bool bRebuildPulseEffect = false;
};
