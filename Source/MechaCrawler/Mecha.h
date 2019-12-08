// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/UserWidget.h"
#include "ActivateWidget.h"
#include "ScanWidget.h"
#include "InventoryWidget.h"
#include "NoteNode.h"
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
	void SetScan();
	void RightMousePressed();
	void LeftMousePressed();
	void SetWayPoint();
	void OpenInventory();
	void Zoom();
	void AddNote();
	void DeleteAllNotes();
	void ZoomIn(float val);
	void ZoomOut(float val);
	void SetCameraView();

	FVector nextLoc;
	FVector currentLoc;

	FQuat currentRot;
	FQuat nextRot;

	FRotator cameraRot;

	UParticleSystemComponent* wayPoint;
	
	//TODO: move into playerhud.h/cpp
	//WIDGET CLASSES
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> useWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> scanWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> inventoryWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ANoteNode> noteWidgetClass;

	UPROPERTY()
	UScanWidget* scanWidget;

	UPROPERTY()
	UActivateWidget* useWidget;

	UPROPERTY()
	UInventoryWidget* inventoryWidget;

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
	float destrutibleDamageAmount;

	UPROPERTY(EditAnywhere)
	float destructibleDamageStrength;

	int forwardAxisIndex;
	int rightAxisIndex;

	FHitResult lookHit;

	FHitResult useHit;
	float useDistance = 150.f;

	FHitResult shootHit;
	float shootDistance = 1000.f;

	FHitResult scanHit;
	float scanDistance = 10000.0f;

	FHitResult moveHit;
	FCollisionQueryParams moveParams;
	float moveDistance = 100.f;
	float traceDistance = 125.f; //If traceDistance is equal to moveDistance, players falls through
	float previousMoveSpeed;
	float maxFallDistance = 10000.f;
	float maxFOV = 110.f;
	float initialZoomFOV = 80.f;

	FVector rootAxes[4];

	bool falling = false;
	bool scanning = false;
	bool zoomed = false;
};
