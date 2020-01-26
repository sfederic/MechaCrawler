// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleBeamInteract.generated.h"

UCLASS()
class MECHACRAWLER_API APuzzleBeamInteract : public AActor
{
	GENERATED_BODY()
	
public:	
	APuzzleBeamInteract();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* onMaterial;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* offMaterial;

	bool bOn;

	UPROPERTY(EditAnywhere)
	bool bTransparent;
};
