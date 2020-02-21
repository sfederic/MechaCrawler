// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TagDistanceWidget.h"
#include "TagNode.generated.h"

UCLASS()
class MECHACRAWLER_API ATagNode : public AActor
{
	GENERATED_BODY()
	
public:	
	ATagNode();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	class UWidgetComponent* tagWidget;
};
