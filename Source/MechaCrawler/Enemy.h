// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridActor.h"
#include "Components/BoxComponent.h"
#include "Enemy.generated.h"

UCLASS()
class MECHACRAWLER_API AEnemy : public AGridActor
{
	GENERATED_BODY()
	
public:	
	AEnemy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	float moveSpeed;
};
