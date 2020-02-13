// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Activate.h"
#include "DialogueUse.generated.h"

//Basically an NPC point can call GetDialogue on each Use()

UCLASS()
class MECHACRAWLER_API ADialogueUse : public AActor, public IActivate
{
	GENERATED_BODY()
	
public:	
	ADialogueUse();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void Use() override;

};
