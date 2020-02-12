// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OverworldEntrance.generated.h"

UCLASS()
class MECHACRAWLER_API AOverworldEntrance : public AActor
{
	GENERATED_BODY()
	
public:	
	AOverworldEntrance();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void LoadLevel(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	class UBoxComponent* box;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString levelName;
};