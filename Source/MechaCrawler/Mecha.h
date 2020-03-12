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
#include "DialogueChoiceWidget.h"
#include "Pickup.h"
#include "TagNode.h"
#include "Memories.h"
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
	void TagActor();
	void RightMousePressed();
	void LeftMousePressed();
	void LeftMousePressedScan(float val);
	void RightMousePressedScan(float val);
	void SetWayPoint();
	void StartLevel();
	void Zoom();
	void AddNote();
	void DeleteAllNotes();
	void ZoomIn(float val);
	void ZoomOut(float val);
	void SetCameraView();
	void RebuildAllDestroyedActors();
	void UseObject();
	void ChangeWeapon();
	void AddDestructibleToRebuildManager(AActor* hitActor);

	UFUNCTION(BlueprintCallable)
	void ProgressText();

	void DashForward();
	void Scan();

	UFUNCTION(BlueprintCallable)
	void GetDialogue(AActor* dialogueActor);

	void ScrollText();

	UPROPERTY(VisibleAnywhere)
	FVector nextLoc;
	UPROPERTY(VisibleAnywhere)
	FVector currentLoc;
	UPROPERTY(VisibleAnywhere)
	FVector lastLoc;

	FQuat currentRot;
	FQuat nextRot;

	FRotator cameraRot;

	UPROPERTY(VisibleAnywhere, Category="Memories")
	TArray<EMemories> memories;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EPickupType> pickups;

	UPROPERTY(EditAnywhere)
	class UMaterialParameterCollection* outlineParams;

	class UCameraComponent* camera;
	class APlayerController* controller;

	UPROPERTY(EditAnywhere, Category="Camera Shakes")
	TSubclassOf<class UCameraShake> cameraShake;

	UPROPERTY(EditAnywhere, Category = "Camera Shakes")
	TSubclassOf<class UCameraShake> beamShootCameraShake;

	UPROPERTY()
	TArray<UActorComponent*> weapons;
	int currentWeaponIndex = 0;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* destroyableWireframeMaterial;	
	
	UPROPERTY(EditAnywhere)
	class APostProcessVolume* postProcessMain;

	UPROPERTY(EditAnywhere)
	UMaterialInstance* scanPostProcess;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* destroyableBaseMaterial;

	//PARTICLES
	UPROPERTY(EditAnywhere, Category="Particles")
	UParticleSystem* explosionParticle;

	UParticleSystemComponent* wayPoint;

	UPROPERTY(EditAnywhere, Category = "Particles")
	UParticleSystem* beamShootParticleTemplate;

	UParticleSystemComponent* beamShootParticle;

	//REBUILDS
	UPROPERTY(EditAnywhere)
	ARebuildManager* instancedRebuildManager;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ADestructibleActor> rebuildActorClass;

	//TODO: move into playerhud.h/cpp
	//WIDGET CLASSES

	UPROPERTY(EditAnywhere, Category="Widgets")
	TSubclassOf<ATagNode> tagWidgetClass;

	UPROPERTY(EditAnywhere, Category="Widgets")
	TSubclassOf<UUserWidget> noteReturnFocusWidgetClass;

	UPROPERTY()
	UUserWidget* noteReturnFocusWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> shootingWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* shootingWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> startLevelWidgetClass;

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

	UPROPERTY(EditAnywhere)
	TSubclassOf<UDialogueChoiceWidget> choiceWidgetClass;

	UPROPERTY()
	UDialogueChoiceWidget* choiceWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ADestructibleActor> iceBlockClass;

	UPROPERTY()
	UTextBoxWidget* textBoxWidget;

	UPROPERTY()
	UScanWidget* scanWidget;

	UPROPERTY()
	UActivateWidget* useWidget;

	UPROPERTY()
	UUserWidget* startLevelWidget;

	UPROPERTY()
	UInventoryWidget* inventoryWidget;
	//end move to playerhud.h/cpp

	//Sounds
	UPROPERTY(EditAnywhere, Category="SFX")
	USoundBase* soundScanOn;

	UPROPERTY(EditAnywhere, Category = "SFX")
	USoundBase* soundScanOff;

	UPROPERTY(EditAnywhere, Category = "SFX")
	USoundBase* soundScanOverlap;

	UPROPERTY(EditAnywhere, Category = "SFX")
	USoundBase* soundZoomIn;

	UPROPERTY(EditAnywhere, Category = "SFX")
	USoundBase* soundZoomOut;

	UPROPERTY(EditAnywhere, Category = "SFX")
	USoundBase* soundDeleteAll;

	UPROPERTY(EditAnywhere, Category = "SFX")
	USoundBase* soundNote;

	UPROPERTY(EditAnywhere, Category = "SFX")
	USoundBase* soundNoteLoseFocus;

	UPROPERTY(EditAnywhere, Category = "SFX")
	USoundBase* soundTagged;

	UPROPERTY(EditAnywhere, Category = "SFX")
	USoundBase* soundTaggedOff;

	UPROPERTY(EditAnywhere, Category = "SFX")
	USoundBase* soundWaypointSet;

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

	UPROPERTY(VisibleAnywhere)
	float shootCooldownTimer = 0.f;

	int forwardAxisIndex;
	int rightAxisIndex;
	int upAxisIndex;

	FHitResult lookHit;

	FHitResult useHit;
	float useDistance = 125.f; //TODO: Figure out whether to use Zelda/Metroid like distance switches

	FHitResult shootHit;

	UPROPERTY(VisibleAnywhere)
	float attackDistance;

	FHitResult scanHit;
	FHitResult previousScanHit;
	float scanDistance = 10000.0f;

	FHitResult moveHit;
	FCollisionQueryParams moveParams;
	float moveDistance = 100.f;
	float traceDistance = 125.f; //If traceDistance is equal to moveDistance, players falls through
	float previousMoveSpeed;
	float maxFallDistance = 500000.f; //TODO: See if this works for other levels as well as planet shield
	float maxFOV = 110.f;
	float initialZoomFOV = 80.f;
	
	UPROPERTY(EditAnywhere)
	float dashSpeed;

	FVector rootAxes[6];

	UPROPERTY(VisibleAnywhere)
	bool falling = false;

	bool scanning = false;
	bool zoomed = false;

	bool bTypingNote = false;

	UPROPERTY(VisibleAnywhere)
	bool submerged = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDialogueClick = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool canMove = true;

	bool bFadeOutRebuild = false;
	float fadeOutTimer = 0.f;

	UPROPERTY(EditAnywhere)
	bool bStartLevelOnShip;
};
