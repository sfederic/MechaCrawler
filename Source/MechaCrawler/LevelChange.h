// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelChange.generated.h"

//Walk into box, level changes. Just for testing

UCLASS()
class MECHACRAWLER_API ALevelChange : public AActor
{
	GENERATED_BODY()
	
public:	
	ALevelChange();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnPlayerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName levelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector levelSpawnLocation;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> levelChangeConfirmWidgetClass;

	UPROPERTY()
	UUserWidget* levelChangeConfirmWidget;

	class UBoxComponent* box;
};
