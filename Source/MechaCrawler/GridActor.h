// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridActor.generated.h"

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
