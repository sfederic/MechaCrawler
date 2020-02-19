// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelSequenceActor.h"
#include "CinematicEventActor.generated.h"

//Pretty much all done with blueprint

UCLASS()
class MECHACRAWLER_API ACinematicEventActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACinematicEventActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void Activate();

	UFUNCTION()
	void OnPlayerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCinematicStop();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsActivated = false;

	UPROPERTY(EditAnywhere)
	ULevelSequence* shot;

	ALevelSequenceActor* sequenceActor;

	class UBoxComponent* box;
};
