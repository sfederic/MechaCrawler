// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Memories.h"
#include "OverworldEntrance.generated.h"

//To be placed over levels that are shown on the overworld map to enter them and setup UI

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

	UPROPERTY(EditAnywhere)
	UMaterialInterface* lockedMaterial;

	UPROPERTY()
	TArray<UMaterialInterface*> originalMaterials;

	UPROPERTY(EditAnywhere)
	EMemories memoryToUnlockLevel;

	class UStaticMeshComponent* mesh;

	class AShip* player;

	UPROPERTY(EditAnywhere)
	bool bLocked;
};
