// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/Classes/Camera/CameraShake.h"
#include "WeaponData.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MECHACRAWLER_API UWeaponData : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWeaponData();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	UParticleSystem* weaponParticle;

	UPROPERTY(EditAnywhere)
	UCameraShake* camShake;

	UPROPERTY(EditAnywhere)
	float damage;

	UPROPERTY(EditAnywhere)
	float range;

	UPROPERTY(EditAnywhere)
	bool explosive;
};
