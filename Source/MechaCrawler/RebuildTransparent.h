// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridActor.h"
#include "Rebuild.h"
#include "RebuildTransparent.generated.h"

//For transparent actors that can be spawned (rebuild mechanically) by player clicks

UCLASS()
class MECHACRAWLER_API ARebuildTransparent : public AGridActor, public IRebuild
{
	GENERATED_BODY()
	
public:	
	ARebuildTransparent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void Rebuild() override;

	void AddOverlap();
	void Build();

	UPROPERTY(EditAnywhere)
	UMaterialInterface* rebuildMaterial;

	UMaterialInterface* originalMaterial;

	UPROPERTY(VisibleAnywhere)
	bool bRebuilt = false;
};
