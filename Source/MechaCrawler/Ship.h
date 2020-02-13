// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnterLevelWidget.h"
#include "TextBoxWidget.h"
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

	UFUNCTION(BlueprintCallable)
	void GetDialogue(AActor* dialogueActor);

	void ProgressText();
	void ScrollText();
	void LeftMousePressed();

public:
	FHitResult shootHit;
	FCollisionQueryParams shootParams;
	class UCameraComponent* camera;
	class APlayerController* controller;

	//Widgets
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> levelEntryWidgetClass;

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

	UPROPERTY(VisibleAnywhere)
	bool bReversing;

	UPROPERTY(VisibleAnywhere)
	bool bMovingForward;

	bool bDialogueClick;
};
