// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AudioTrigger.generated.h"

//Trigger box that plays a one shot audio clip. Also takes in a 3d location for PlaySoundAtlocation()

UCLASS()
class MECHACRAWLER_API AAudioTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	AAudioTrigger();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* soundOrigin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsOneShot;
};
