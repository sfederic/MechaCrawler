// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractiveActor.h"
#include "Activate.h"
#include "Scannable.h"
#include "ScanData.h"
#include "Door.generated.h"


UCLASS()
class MECHACRAWLER_API ADoor : public AActor, public IActivate, public IScannable
{
	GENERATED_BODY()
	
public:	
	ADoor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void Use() override;

	UFUNCTION()
	virtual UScanData* Scan() override;

	FVector currentLoc;
	FVector nextLoc;

	UPROPERTY(EditAnywhere)
	float openSpeed;

	bool openState;
};
