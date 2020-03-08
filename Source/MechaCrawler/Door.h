// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GridActor.h"
#include "Activate.h"
#include "Scannable.h"
#include "ScanData.h"
#include "Rebuild.h"
#include "Door.generated.h"


UCLASS()
class MECHACRAWLER_API ADoor : public AGridActor, public IActivate, public IScannable, public IRebuild
{
	GENERATED_BODY()
	
public:	
	ADoor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void Rebuild() override;

	UFUNCTION(BlueprintCallable)
	virtual void Use() override;

	UFUNCTION()
	virtual UScanData* Scan() override;

	UPROPERTY(EditAnywhere)
	USoundBase* openSound;

	UPROPERTY(EditAnywhere)
	USoundBase* closeSound;

	UPROPERTY(EditAnywhere)
	FVector openDirection;

	UPROPERTY(EditAnywhere)
	float openSpeed;

	UPROPERTY(EditAnywhere)
	float openDistance;

	UPROPERTY(EditAnywhere)
	bool openState;
};
