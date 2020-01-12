// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridActor.h"
#include "AirEnemy.generated.h"

UCLASS()
class MECHACRAWLER_API AAirEnemy : public AGridActor
{
	GENERATED_BODY()

public:
	AAirEnemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
