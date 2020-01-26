// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridActor.h"
#include "PuzzleBeam.generated.h"

UCLASS()
class MECHACRAWLER_API APuzzleBeam : public AGridActor
{
	GENERATED_BODY()
	
public:	
	APuzzleBeam();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	FHitResult beamHit;
	FCollisionQueryParams beamParams;

	UPROPERTY(EditAnywhere)
	UParticleSystem* beamTemplate;

	class UParticleSystemComponent* beam;

	UPROPERTY(VisibleAnywhere)
	class APuzzleBeamInteract* previousActor;

	UPROPERTY(EditAnywhere)
	float beamDistance;
};
