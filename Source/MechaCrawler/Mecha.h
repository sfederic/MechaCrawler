// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/UserWidget.h"
#include "ActivateWidget.h"
#include "ScanWidget.h"
#include "InventoryWidget.h"
#include "TextBoxWidget.h"
#include "TextBox.h"
#include "NoteNode.h"
#include "RebuildManager.h"
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
	void MoveUp(float val);
	void MoveDown(float val);
	void LookYaw(float val);
	void LookPitch(float val);
	void SetScan();
	void RightMousePressed();
	void LeftMousePressed(float val);
	void SetWayPoint();
	void OpenInventory();
	void Zoom();
	void AddNote();
	void DeleteAllNotes();
	void ZoomIn(float val);
	void ZoomOut(float val);
	void SetCameraView();
	void RebuildAllDestroyedActors();
	void UseObject();
	void ChangeWeapon();
	void ProgressText();
	void DashForward();
	void RebuildTimers();

	FVector nextLoc;
	FVector currentLoc;

	FQuat currentRot;
	FQuat nextRot;

	FRotator cameraRot;

	class UCameraComponent* camera;
	class APlayerController* controller;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShake> cameraShake;

	UPROPERTY()
	TArray<UActorComponent*> weapons;
	int currentWeaponIndex = 0;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* destroyableWireframeMaterial;	
	
	UPROPERTY(EditAnywhere)
	UMaterialInterface* destroyableBaseMaterial;

	//PARTICLES
	UPROPERTY(EditAnywhere)
	UParticleSystem* explosionParticle;
	UParticleSystemComponent* wayPoint;

	//REBUILDS
	UPROPERTY(EditAnywhere)
	ARebuildManager* instancedRebuildManager;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ADestructibleActor> rebuildActorClass;

	//TODO: move into playerhud.h/cpp
	//WIDGET CLASSES
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> textBoxWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> useWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> scanWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> inventoryWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ANoteNode> noteWidgetClass;

	UPROPERTY()
	UTextBoxWidget* textBoxWidget;
	int textBoxIndex = 0;
	TArray<FTextBox*> textBoxRows;

	UPROPERTY()
	UScanWidget* scanWidget;

	UPROPERTY()
	UActivateWidget* useWidget;

	UPROPERTY()
	UInventoryWidget* inventoryWidget;
	//end move to playerhud.h/cpp

	UPROPERTY(VisibleAnywhere)
	FVector forwardAxis;

	UPROPERTY(VisibleAnywhere)
	FVector rightAxis;

	UPROPERTY(EditAnywhere)
	float moveSpeed;

	float initialMoveSpeed;

	UPROPERTY(EditAnywhere)
	float cameraSpeed;

	UPROPERTY(EditAnywhere)
	float rotateSpeed;

	UPROPERTY(EditAnywhere)
	float destructibleDamageAmount;

	UPROPERTY(EditAnywhere)
	float destructibleDamageStrength;

	int forwardAxisIndex;
	int rightAxisIndex;
	int upAxisIndex;

	FHitResult lookHit;

	FHitResult useHit;
	float useDistance = 175.f;

	FHitResult shootHit;

	UPROPERTY(VisibleAnywhere)
	float attackDistance;

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
	
	UPROPERTY(EditAnywhere)
	float dashSpeed;

	FVector rootAxes[6];

	UPROPERTY(VisibleAnywhere)
	bool falling = false;

	bool scanning = false;
	bool zoomed = false;

	UPROPERTY(VisibleAnywhere)
	bool submerged = false;

	bool canMove = true;
};
