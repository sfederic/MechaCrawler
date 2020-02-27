// Fill out your copyright notice in the Description page of Project Settings.

#include "WallEnemy.h"
#include "Engine/World.h"
#include "Misc/App.h"
#include "GlobalTags.h"
#include "Components/BoxComponent.h"

AWallEnemy::AWallEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	moveParams.AddIgnoredActor(this);

	moveTimer = FMath::RandRange(0.f, 2.f);
}

void AWallEnemy::BeginPlay()
{
	Super::BeginPlay();

	currentLoc = GetActorLocation();
	nextLoc = GetActorLocation();

	moveAxes[0] = GetActorForwardVector();
	moveAxes[1] = -GetActorForwardVector();
	moveAxes[2] = GetActorRightVector();
	moveAxes[3] = -GetActorRightVector();

	boxCollision = FindComponentByClass<UBoxComponent>();
	boxCollision->OnComponentBeginOverlap.AddDynamic(this, &AWallEnemy::OnPlayerOverlapBegin);
}

void AWallEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	currentLoc = GetActorLocation();

	moveAxes[0] = GetActorForwardVector();
	moveAxes[1] = -GetActorForwardVector();
	moveAxes[2] = GetActorRightVector();
	moveAxes[3] = -GetActorRightVector();

	if (moveTimer > 2.0f && GetActorLocation().Equals(nextLoc))
	{
		//Check below enemy
		FHitResult baseHit;
		if (GetWorld()->LineTraceSingleByChannel(baseHit, GetActorLocation(), GetActorLocation() - GetActorUpVector() * 125.f, ECC_WorldStatic, moveParams))
		{
			//Check for movement hits
			int32 randomMoveIndex = FMath::RandRange(0, 3);
			FHitResult moveHit;

			if (!GetWorld()->LineTraceSingleByChannel(moveHit, GetActorLocation(), GetActorLocation() + moveAxes[randomMoveIndex] * 125.f, ECC_WorldStatic, moveParams))
			{
				nextLoc = GetActorLocation() + (moveAxes[randomMoveIndex] * moveDistance);
				nextLoc.X = FMath::RoundToFloat(nextLoc.X);
				nextLoc.Y = FMath::RoundToFloat(nextLoc.Y);
				nextLoc.Z = FMath::RoundToFloat(nextLoc.Z);

				if (GetWorld()->LineTraceSingleByChannel(moveHit, nextLoc, nextLoc - GetActorUpVector() * (125.f), ECC_WorldStatic, moveParams))
				{
					if (rebuildManager)
					{
						for (int i = 0; i < rebuildManager->movingGridActors.Num(); i++)
						{
							AGridActor* gridActor = Cast<AGridActor>(rebuildManager->movingGridActors[i]);
							if (gridActor)
							{
								if (nextLoc.Equals(gridActor->nextLoc) && !currentLoc.Equals(gridActor->currentLoc)) 
								{
									nextLoc = GetActorLocation();
									goto endMovement;
								}
							}
						}
					}

					moveTimer = 0.f;
				}
				else
				{
					nextLoc = GetActorLocation();
				}
			}
		}
	}
	else
	{
		moveTimer += FApp::GetDeltaTime();
	}

endMovement:

	SetActorLocation(FMath::VInterpConstantTo(currentLoc, nextLoc, DeltaTime, moveSpeed));
}

void AWallEnemy::DropLoot()
{
	Tags.Add(Tags::Pickup);
	Tags.Add(Tags::Useable);
}
