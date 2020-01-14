// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestructibleActor.h"
#include "GridActor.generated.h"

/*
* AGridActor is only for moveable actors that need a next location
*/

UENUM()
enum class EDirections : uint8
{
	UP UMETA(DisplayName = "Up"),
	DOWN UMETA(DisplayName = "Down"),
	RIGHT UMETA(DisplayName = "Right"),
	LEFT UMETA(DisplayName = "Left"),
	FORWARD UMETA(DisplayName = "Foward"),
	BACK UMETA(DisplayName = "Back")
};

UCLASS()
class MECHACRAWLER_API AGridActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AGridActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere)
	FVector currentLoc;
	UPROPERTY(VisibleAnywhere)
	FVector nextLoc;
};
