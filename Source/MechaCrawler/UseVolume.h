// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UseVolume.generated.h"

UCLASS()
class MECHACRAWLER_API AUseVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	AUseVolume();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void ActivateConnections();

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	class UBoxComponent* volume;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> connectedActors;
};
