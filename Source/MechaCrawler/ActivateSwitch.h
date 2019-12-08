// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Activate.h"
#include "ActivateSwitch.generated.h"

UCLASS()
class MECHACRAWLER_API AActivateSwitch : public AActor, public IActivate
{
	GENERATED_BODY()
	
public:	
	AActivateSwitch();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void Use() override;

	UPROPERTY(EditAnywhere)
	AActor* connectedActor;
};
