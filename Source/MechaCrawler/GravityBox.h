// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridActor.h"
#include "GravityBox.generated.h"

//Actor that moves in an elevator fashion along any axis

UCLASS()
class MECHACRAWLER_API AGravityBox : public AGridActor
{
	GENERATED_BODY()
	
public:	
	AGravityBox();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	FVector gravityDirection;

	UPROPERTY(EditAnywhere)
	float fallSpeed;
};
