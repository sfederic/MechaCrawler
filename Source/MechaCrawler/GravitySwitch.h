// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridActor.h"
#include "Activate.h"
#include "GravitySwitch.generated.h"

UCLASS()
class MECHACRAWLER_API AGravitySwitch : public AGridActor, public IActivate
{
	GENERATED_BODY()
	
public:	
	AGravitySwitch();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void Use() override;

	UPROPERTY(EditAnywhere)
	EDirections gravity;
};
