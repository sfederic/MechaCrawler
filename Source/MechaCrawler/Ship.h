// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnterLevelWidget.h"
#include "TextBoxWidget.h"
#include "NoteNode.h"
#include "ScanWidget.h"
#include "Memories.h"
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
	void RotateUp(float val);
	void RotateDown(float val);
	void Accelerate(float val);
	void Reverse(float val);
	void SetScan();
	void TagActor();
	void AddNote();
	void Scan();

	UFUNCTION(BlueprintCallable)
	void GetDialogue(AActor* dialogueActor);

	void ProgressText();
	void ScrollText();
	void LeftMousePressed();
	void RightMousePressed();

public:
	FHitResult shootHit;
	FHitResult scanHit, previousScanHit;
	FCollisionQueryParams shootParams;
	class UCameraComponent* camera;
	class APlayerController* controller;

	UPROPERTY(VisibleAnywhere, Category="Memories")
	TArray<EMemories> memories;

	//Widgets
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> levelEntryWidgetClass;

	UPROPERTY(EditAnywhere, Category="Widget")
	TSubclassOf<ANoteNode> noteWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UUserWidget> noteReturnFocusWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<class ATagNode> tagWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UUserWidget> scanCursorWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UScanWidget> scanWidgetClass;

	UPROPERTY()
	UScanWidget* scanWidget;

	UPROPERTY()
	UUserWidget* scanCursorWidget;

	UPROPERTY()
	UUserWidget* noteReturnFocusWidget;

	UPROPERTY()
	UEnterLevelWidget* levelEntryWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> textBoxWidgetClass;

	UPROPERTY()
	UTextBoxWidget* textBoxWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString levelToEnterName; //This is here because I can't figure out how to get an actors string from a widget to its class without GetPlayerPawn()

	UPROPERTY(EditAnywhere)
	float velocityMin;

	UPROPERTY(EditAnywhere)
	float velocityMax;

	UPROPERTY(EditAnywhere)
	float reverseVelocityMin;

	UPROPERTY(EditAnywhere)
	float reverseVelocityMax;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float velocity;

	UPROPERTY(VisibleAnywhere)
	float reverseVelocity;

	UPROPERTY(EditAnywhere)
	float shipMoveSpeed;	

	UPROPERTY(EditAnywhere)
	float reverseSpeed;
	
	UPROPERTY(EditAnywhere)
	float shipRotateSpeed;

	float shootDistance = 5000.f;

	float scanDistance = 10000.f;

	UPROPERTY(VisibleAnywhere)
	bool bReversing;

	bool bIsScanning;

	UPROPERTY(VisibleAnywhere)
	bool bMovingForward;

	bool bDialogueClick;

	bool bTypingNote = false;
};
