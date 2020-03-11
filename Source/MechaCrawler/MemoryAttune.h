// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Activate.h"
#include "Memories.h"
#include "MemoryAttune.generated.h"

UCLASS()
class MECHACRAWLER_API AMemoryAttune : public AActor, public IActivate
{
	GENERATED_BODY()
	
public:	
	AMemoryAttune();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void Use() override;
	void UnpauseLevelSong();

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> attuneWidgetClass;

	UPROPERTY()
	UUserWidget* attuneWidget;

	UPROPERTY(EditAnywhere)
	USoundBase* attuneSound;

	UPROPERTY(EditAnywhere)
	EMemories memoryName;
};
