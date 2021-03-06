// Fill out your copyright notice in the Description page of Project Settings.

#include "NoteNode.h"

ANoteNode::ANoteNode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ANoteNode::BeginPlay()
{
	Super::BeginPlay();

	widgetComponent = FindComponentByClass<UWidgetComponent>();
	noteWidget = Cast<UNoteWidget>(widgetComponent->GetUserWidgetObject());
}

void ANoteNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bFirstSpawn == false)
	{
		noteWidget->note = noteText;
		bFirstSpawn = true;
	}

	noteText = noteWidget->note;
}
