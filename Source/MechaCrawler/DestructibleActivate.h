// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DestructibleActor.h"
#include "Activate.h"
#include "DestructibleActivate.generated.h"

/**
 * 
 */
UCLASS()
class MECHACRAWLER_API ADestructibleActivate : public ADestructibleActor, public IActivate
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void Use() override;

	UDestructibleComponent* dc;
};
