// Fill out your copyright notice in the Description page of Project Settings.


#include "TagNode.h"
#include "Components/WidgetComponent.h"

ATagNode::ATagNode()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ATagNode::BeginPlay()
{
	Super::BeginPlay();
	
	tagWidget = FindComponentByClass<UWidgetComponent>();
}

void ATagNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

