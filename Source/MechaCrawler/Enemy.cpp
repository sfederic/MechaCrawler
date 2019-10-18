// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

float moveTimer;
FVector nextLoc;

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

}


void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	nextLoc = GetActorLocation();
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	moveTimer += FApp::GetDeltaTime();
	if (moveTimer > 3.0f)
	{
		moveTimer = 0.f;

		/*FVector center = levelBounds->Bounds.GetBox().GetCenter();
		GLog->Logf(TEXT("center %f %f %f"), center.X, center.Y, center.Z);
		FVector extent = levelBounds->Bounds.GetBox().GetExtent();
		GLog->Logf(TEXT("extent %f %f %f"), extent.X, extent.Y, extent.Z);*/
		
		FVector offset = FMath::RandPointInBox(levelBounds->Bounds.GetBox());
		offset.X = FMath::RoundToFloat(offset.X);
		offset.Y = FMath::RoundToFloat(offset.Y);
		offset.Z = FMath::RoundToFloat(offset.Z);

		GLog->Logf(TEXT("loc %f %f %f"), offset.X, offset.Y, offset.Z);

		FHitResult hit;
		if (GetWorld()->LineTraceSingleByChannel(hit, offset, offset, ECC_WorldStatic))
		{
			return;
		}
		else 
		{
			nextLoc = offset;
		}
	}

	FMath::VInterpConstantTo(GetActorLocation(), nextLoc, DeltaTime, 300.0f);
}

