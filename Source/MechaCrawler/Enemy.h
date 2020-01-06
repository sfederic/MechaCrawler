// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridActor.h"
#include "EnemyHealthBarWidget.h"
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

	FCollisionQueryParams params;
	FHitResult hit;

	class UWidgetComponent* healthBarComponent;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> healthBarClass;

	UEnemyHealthBarWidget* healthBar;

	UPROPERTY(EditAnywhere)
	float moveSpeed;

	UPROPERTY(EditAnywhere)
	float moveTimer;	

	float killTimer;

	bool kill = false;
};
