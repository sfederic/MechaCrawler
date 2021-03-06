// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IceBlock.generated.h"

UCLASS()
class MECHACRAWLER_API AIceBlock : public AActor
{
	GENERATED_BODY()
	
public:	
	AIceBlock();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
