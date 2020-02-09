// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EventActor.h"
#include "DialogueBox.generated.h"

//Player walks inside collision component, gets dialogue.Inhertis from AActor to save the cast in GetDialogue()

UCLASS()
class MECHACRAWLER_API ADialogueBox : public AActor
{
	GENERATED_BODY()
	
public:	
	ADialogueBox();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnPlayerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void Activate();

	UPROPERTY(VisibleAnywhere)
	bool bHasBeenRead = false;

	UPROPERTY(EditAnywhere)
	bool bIsActivated = true;

	UPROPERTY(EditAnywhere)
	TArray<ADialogueBox*> dialogueBoxesToActivate;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> actorsToActivate;

	UPROPERTY(EditAnywhere)
	TArray<AEventActor*> eventActors;

	class UBoxComponent* box;
};
