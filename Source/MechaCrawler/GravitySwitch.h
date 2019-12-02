// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridActor.h"
#include "Activate.h"
#include "GravitySwitch.generated.h"

UENUM()
enum class EDirections : uint8
{
	UP UMETA(DisplayName="Up"),
	DOWN UMETA(DisplayName = "Down"),
	RIGHT UMETA(DisplayName = "Right"),
	LEFT UMETA(DisplayName = "Left"),
	FORWARD UMETA(DisplayName = "Foward"),
	BACK UMETA(DisplayName = "Back")
};

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
