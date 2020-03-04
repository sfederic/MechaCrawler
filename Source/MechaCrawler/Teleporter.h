// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Teleporter.generated.h"

//Walk into this actor and transport the player to another teleporter or location. Stole idea from Shin Megami Strange Journey

UCLASS()
class MECHACRAWLER_API ATeleporter : public AActor
{
	GENERATED_BODY()
	
public:	
	ATeleporter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	AActor* actorToTeleportTo;

	class APawn* playerPawn;
};
