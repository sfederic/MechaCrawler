// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/UserWidget.h"
#include "Mecha.generated.h"

UCLASS()
class MECHACRAWLER_API AMecha : public APawn
{
	GENERATED_BODY()

public:
	AMecha();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void MoveForward(float val);
	void MoveBack(float val);
	void MoveLeft(float val);
	void MoveRight(float val);
	void LookYaw(float val);
	void LookPitch(float val);

	FVector nextLoc;
	FVector currentLoc;

	FQuat currentRot;
	FQuat nextRot;

	FRotator cameraRot;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> widgetClass;

	UPROPERTY()
	UUserWidget* useWidget;

	UPROPERTY(VisibleAnywhere)
	FVector forwardAxis;

	UPROPERTY(VisibleAnywhere)
	FVector rightAxis;

	UPROPERTY(EditAnywhere)
	float moveSpeed;

	UPROPERTY(EditAnywhere)
	float cameraSpeed;

	UPROPERTY(EditAnywhere)
	float rotateSpeed;

	UPROPERTY(EditAnywhere)
	float damageAmount;

	UPROPERTY(EditAnywhere)
	float damageStrength;

	int forwardAxisIndex;
	int rightAxisIndex;

	FHitResult lookHit;

	FHitResult useHit;
	float useDistance = 150.f;

	FHitResult shootHit;
	float shootDistance = 1000.f;

	FHitResult moveHit;
	FCollisionQueryParams moveParams;
	float moveDistance = 100.f;
	float traceDistance = 125.f; //If traceDistance is equal to moveDistance, players falls through
	float previousMoveSpeed;

	FVector rootAxes[4];

	bool falling = false;
};
