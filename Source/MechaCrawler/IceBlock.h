// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Activate.h"
#include "IceBlock.generated.h"

UCLASS()
class MECHACRAWLER_API AIceBlock : public AActor, public IActivate
{
	GENERATED_BODY()
	
public:	
	AIceBlock();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void Use() override;
	void TurnToIce(); //Used for resetting in outisde classes/functiorinos

	UPROPERTY(EditAnywhere)
	UMaterialInterface* iceMaterial;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* waterMaterial;

	UPROPERTY(EditAnywhere)
	bool isIce = true;

private:
	class UMeshComponent* iceBlockMesh;
	class UScanData* iceBlockScanData;

	FVector iceScale;
	FVector waterScale;
};
