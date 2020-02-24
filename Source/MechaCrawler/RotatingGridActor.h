// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridActor.h"
#include "Rebuild.h"
#include "RotatingGridActor.generated.h"

UCLASS()
class MECHACRAWLER_API ARotatingGridActor : public AGridActor, public IRebuild
{
	GENERATED_BODY()
	
public:	
	ARotatingGridActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void Rebuild() override;

};
