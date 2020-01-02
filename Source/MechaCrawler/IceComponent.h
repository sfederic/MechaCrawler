// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IceComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MECHACRAWLER_API UIceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UIceComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void TurnToWater();
	void TurnToIce();

	UPROPERTY(EditAnywhere)
	class UMaterialInterface* iceMaterial;

	UPROPERTY(EditAnywhere)
	class UMaterialInterface* waterMaterial;

	UPROPERTY(EditAnywhere)
	bool isIce = true;

private:
	class UMeshComponent* iceBlockMesh;
	class UScanData* iceBlockScanData;

	FVector iceScale;
	FVector waterScale;

	const float meltSpeed = 2.0f;
};
