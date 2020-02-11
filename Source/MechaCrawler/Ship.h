// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnterLevelWidget.h"
#include "Ship.generated.h"

//Pawn used to travel world map

UCLASS()
class MECHACRAWLER_API AShip : public APawn
{
	GENERATED_BODY()

public:
	AShip();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void RotateRight(float val);
	void RotateLeft(float val);
	void Accelerate(float val);
	void Reverse(float val);

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> levelEntryWidgetClass;

	UEnterLevelWidget* levelEntryWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString levelToEnterName; //This is here because I can't figure out how to get an actors string from a widget to its class without GetPlayerPawn()

	float velocityMin = 0.f;
	float velocityMax = 13.5f;

	float reverseVelocityMin = -7.5f;
	float reverseVelocityMax = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float velocity;

	UPROPERTY(VisibleAnywhere)
	float reverseVelocity;

	UPROPERTY(EditAnywhere)
	float moveSpeed;	

	UPROPERTY(EditAnywhere)
	float reverseSpeed;
	
	UPROPERTY(EditAnywhere)
	float rotateSpeed;

	bool bReversing;
};
