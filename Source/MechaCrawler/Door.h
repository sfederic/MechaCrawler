// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GridActor.h"
#include "Activate.h"
#include "Scannable.h"
#include "ScanData.h"
#include "Door.generated.h"


UCLASS()
class MECHACRAWLER_API ADoor : public AGridActor, public IActivate, public IScannable
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

	UPROPERTY(EditAnywhere)
	FVector openDirection;
	UPROPERTY(EditAnywhere)
	float openSpeed;
	float openDistance = 100.f;
	bool openState;
};
