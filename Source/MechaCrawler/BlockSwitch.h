// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlockSwitch.generated.h"

/*
* Acts as a pressure plate for blocks. OnActorOverlap/End didn't work for this kind of functionality.
*/

UCLASS()
class MECHACRAWLER_API ABlockSwitch : public AActor
{
	GENERATED_BODY()
	
public:	
	ABlockSwitch();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	void ActivateConnection();

	UPROPERTY(EditAnywhere)
	UMaterialInterface* activatedMat;	
	
	UPROPERTY(EditAnywhere)
	UMaterialInterface* deactivatedMat;

	class UMeshComponent* mesh;
	class APawn* player;
	TArray<AActor*> pushableActors;

	UPROPERTY(EditAnywhere)
	AActor* connectedActor; //Acts as the actor the swithc is connected to (doors, switches, etc.)

	bool activated = false;
};
