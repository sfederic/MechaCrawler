// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridActor.h"
#include "ShieldEnemy.generated.h"

UCLASS()
class MECHACRAWLER_API AShieldEnemy : public AGridActor
{
	GENERATED_BODY()
	
public:	
	AShieldEnemy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	class APawn* player;

	UPROPERTY(EditAnywhere)
	float rotateSpeed;
};
