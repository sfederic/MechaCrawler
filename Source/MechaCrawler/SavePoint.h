// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SavePoint.generated.h"

UCLASS()
class MECHACRAWLER_API ASavePoint : public AActor
{
	GENERATED_BODY()
	
public:	
	ASavePoint();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnPlayerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	class UBoxComponent* box;

	UPROPERTY(VisibleAnywhere)
	FString saveSlot;

	int saveSlotIndex = 0;
};
