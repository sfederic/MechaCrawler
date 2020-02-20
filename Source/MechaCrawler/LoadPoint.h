// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NoteNode.h"
#include "LoadPoint.generated.h"

//This just exists because I can't use fucking blueprints. Otherwise would be in level blueprints EventBegin

UCLASS()
class MECHACRAWLER_API ALoadPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	ALoadPoint();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ANoteNode> noteWidgetClass;

	UPROPERTY(VisibleAnywhere)
	FString loadSlot;

	int loadSlotIndex = 0;
};
