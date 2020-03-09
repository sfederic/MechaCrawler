// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Atmosphere/AtmosphericFogComponent.h"
#include "AtmosphereChanger.generated.h"

//Actor to change atmospheric fog distance scale to get an out of atmosphere effect. 4.24's SkyAtmosphere didn't cut it.

UCLASS()
class MECHACRAWLER_API AAtmosphereChanger : public AActor
{
	GENERATED_BODY()
	
public:	
	AAtmosphereChanger();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void ChangeDistance(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UAtmosphericFogComponent* fog;
	class UBoxComponent* box;

	UPROPERTY(EditAnywhere)
	float distanceIncrement;

	UPROPERTY(EditAnywhere)
	float maxDistance; //70 seems to look good;

	UPROPERTY(VisibleAnywhere)
		float dist;

	bool bIncrementDensity;
};
