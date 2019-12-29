// Fill out your copyright notice in the Description page of Project Settings.

#include "IceBlock.h"
#include "Components/MeshComponent.h"
#include "ScanData.h"
#include "GlobalTags.h"

AIceBlock::AIceBlock()
{
	PrimaryActorTick.bCanEverTick = true;

	iceScale = FVector(1.f);
	waterScale = FVector(0.9f);
}

void AIceBlock::BeginPlay()
{
	Super::BeginPlay();

	iceBlockMesh = FindComponentByClass<UMeshComponent>();
	check(iceBlockMesh);

	iceBlockScanData = FindComponentByClass<UScanData>();
	check(iceBlockScanData);

	if (isIce && iceMaterial)
	{
		iceBlockMesh->SetMaterial(0, iceMaterial);
	}
	else if(!isIce && waterMaterial)
	{
		Tags.Add(Tags::MoveThrough);
		iceBlockMesh->SetMaterial(0, waterMaterial);
	}
}

void AIceBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float meltSpeed = 2.0f;
	if(!isIce)
	{
		SetActorScale3D(FMath::VInterpConstantTo(GetActorScale(), waterScale, DeltaTime, meltSpeed));
	}
	else if (isIce)
	{
		SetActorScale3D(FMath::VInterpConstantTo(GetActorScale(), iceScale, DeltaTime, meltSpeed));
	}
}

void AIceBlock::Use()
{
	Tags.Empty();

	if (iceBlockMesh && waterMaterial && iceMaterial)
	{
		if (isIce) //Set as water
		{
			Tags.Add(Tags::MoveThrough);
			iceBlockMesh->SetMaterial(0, waterMaterial);
			iceBlockScanData->scanName = TEXT("Water");
			iceBlockScanData->scanText = TEXT("Player can move through water. Also is wet.");
			SetActorEnableCollision(false);
			isIce = false;
		}
		else if(!isIce) //Set as ice
		{
			TurnToIce();
		}
	}
}

void AIceBlock::TurnToIce()
{
	if (!isIce)
	{
		iceBlockMesh->SetMaterial(0, iceMaterial);
		iceBlockScanData->scanName = TEXT("Ice");
		iceBlockScanData->scanText = TEXT("Currently Hard. Can be melted.");
		SetActorEnableCollision(true);
		isIce = true;
	}
}
