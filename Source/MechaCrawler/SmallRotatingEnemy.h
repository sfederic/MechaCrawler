// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "SmallRotatingEnemy.generated.h"

/**
 * Smaller version of rotating boss
 */

UCLASS()
class MECHACRAWLER_API ASmallRotatingEnemy : public AEnemy
{
	GENERATED_BODY()
public:
    ASmallRotatingEnemy();
protected:
    virtual void BeginPlay();
public:
    virtual void Tick(float DeltaTime) override;
    virtual void ActivateHitEffect() override;
    void DestroyDestructible();

};