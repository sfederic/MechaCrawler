// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Activate.h"
#include "Pickup.generated.h"

UCLASS()
class MECHACRAWLER_API APickup : public AActor, public IActivate
{
	GENERATED_BODY()
	
public:	
	APickup();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void Use() override;
	void AddToInventory();

	UPROPERTY(EditAnywhere)
	FString name;

	UPROPERTY(VisibleAnywhere)
	bool isInInventory;
};
