// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Activate.h"
#include "Rebuild.h"
#include "PuzzleItem.h"
#include "UseOrderManager.h"
#include "ConstructBeacon.generated.h"

//Useable actor that will light up when Use()'d. Figured would work well with "Press them in order" puzzles

UCLASS()
class MECHACRAWLER_API AConstructBeacon : public APuzzleItem
{
	GENERATED_BODY()
	
public:	
	AConstructBeacon();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void Use() override;
	virtual void Rebuild() override;

	UPROPERTY(EditAnywhere)
	USoundBase* activateSound;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* activatedMaterial;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* originalMaterial;

	UPROPERTY(EditAnywhere)
	AUseOrderManager* useManager;
};
