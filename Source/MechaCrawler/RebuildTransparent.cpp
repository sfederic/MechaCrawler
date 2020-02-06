// Fill out your copyright notice in the Description page of Project Settings.

#include "RebuildTransparent.h"
#include "Components/MeshComponent.h"
#include "Components/BoxComponent.h"

ARebuildTransparent::ARebuildTransparent()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARebuildTransparent::BeginPlay()
{
	Super::BeginPlay();
	
	originalMaterial = FindComponentByClass<UMeshComponent>()->GetMaterial(0);
}

void ARebuildTransparent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARebuildTransparent::Rebuild()
{
	UMeshComponent* mesh = this->FindComponentByClass<UMeshComponent>();
	mesh->SetMaterial(0, originalMaterial);
	mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	mesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
	bRebuilt = false;
	FindComponentByClass<UBoxComponent>()->OnComponentBeginOverlap.RemoveAll(this);
}

void ARebuildTransparent::AddOverlap()
{
	FindComponentByClass<UBoxComponent>()->OnComponentBeginOverlap.AddDynamic(this, &ARebuildTransparent::OnPlayerOverlapBegin);
}
