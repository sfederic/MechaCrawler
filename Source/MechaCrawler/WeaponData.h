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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* weaponParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCameraShake> camShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* decal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float cooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool explosive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ice;
};
