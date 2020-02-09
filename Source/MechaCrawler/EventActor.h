// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EventActor.generated.h"

//Pretty much all done with blueprint

UCLASS()
class MECHACRAWLER_API AEventActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AEventActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void Activate();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsActivated = false;
};
