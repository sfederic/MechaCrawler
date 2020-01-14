// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChaseEnemy.generated.h"

UCLASS()
class MECHACRAWLER_API AChaseEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	AChaseEnemy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
