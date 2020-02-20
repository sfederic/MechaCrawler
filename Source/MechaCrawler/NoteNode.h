// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NoteWidget.h"
#include "Components/WidgetComponent.h"
#include "NoteNode.generated.h"

//TODO: Keep for now instead of using straight Actor. might need widget text for maps system

UCLASS()
class MECHACRAWLER_API ANoteNode : public AActor
{
	GENERATED_BODY()
	
public:	
	ANoteNode();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UNoteWidget* noteWidget;
	UWidgetComponent* widgetComponent;

	UPROPERTY(VisibleAnywhere)
	FString noteText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bFirstSpawn = true;
};
