// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestructibleActor.h"
#include "DestructibleComponent.h"
#include "RebuildManager.generated.h"

UCLASS()
class MECHACRAWLER_API ARebuildManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ARebuildManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	//TODO: Delcaring UPROPERTY() keeps the array garbage collected. Keeping it off for now.
	UPROPERTY(VisibleAnywhere)
	TArray<ADestructibleActor*> rebuildActors;
	UPROPERTY(VisibleAnywhere)
	TArray<UDestructibleMesh*> rebuildMeshes;
	UPROPERTY(VisibleAnywhere)
	TArray<UMaterialInterface*> rebuildMaterials;
};
