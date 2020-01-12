// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "DestructibleComponent.h"
#include "Components/WidgetComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	params.AddIgnoredActor(this);

	killTimer = 0.f;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	nextLoc = GetActorLocation();

	healthBarComponent = FindComponentByClass<UWidgetComponent>();
	check(healthBarComponent);

	healthBar = CreateWidget<UEnemyHealthBarWidget>(GetWorld(), healthBarClass);
	if (healthBar)
	{
		healthBar->health = 1.0f;
		healthBarComponent->SetWidget(healthBar);
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (kill)	
	{
		killTimer += FApp::GetDeltaTime();

		if (killTimer > 4.0f)
		{
			Destroy();
		}
	}

	//TODO: Should this be moved to Mecha.cpp?
	if (healthBar)
	{
		if ((healthBar->health <= 0.f) && (kill == false))
		{
			UDestructibleComponent* dc = FindComponentByClass<UDestructibleComponent>();
			if (dc)
			{
				dc->ApplyDamage(1000.f, dc->GetComponentLocation(), dc->GetOwner()->GetActorForwardVector(), 10000.f);
				//dc->GetOwner()->SetLifeSpan(1.f); //Some reason didn't work
				healthBarComponent->SetHiddenInGame(true);
				kill = true;
			}
		}
	}

	if (!kill)
	{
		moveTimer += FApp::GetDeltaTime();

		//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 125.f, FColor::Red);
		//DrawDebugLine(GetWorld(), nextLoc, nextLoc - (GetActorUpVector() * 125.f), FColor::Purple);

		if (moveTimer > 1.5f && nextLoc.Equals(GetActorLocation()))
		{
			moveTimer = 0.f;

			if (GetWorld()->LineTraceSingleByChannel(hit, GetActorLocation(), GetActorLocation() + (GetActorForwardVector() * 125.f), ECC_WorldStatic, params))
			{
				float randomRotations[4] = { 90.f, 180.f, 270.f, 360.f };
				int rotationIndex = (int)FMath::RandRange(0.f, 4.f);
				SetActorRotation(GetActorRotation() + FRotator(0.f, randomRotations[rotationIndex], 0.f));
			}
			else
			{
				//redoMove:

				float randomRotations[4] = { 90.f, 180.f, 270.f, 360.f };
				int rotationIndex = (int)FMath::RandRange(0.f, 4.f);

				nextLoc = GetActorLocation() + GetActorForwardVector() * 100.f;
				nextLoc.X = FMath::RoundToFloat(nextLoc.X);
				nextLoc.Y = FMath::RoundToFloat(nextLoc.Y);
				nextLoc.Z = FMath::RoundToFloat(nextLoc.Z);
				SetActorRotation(GetActorRotation() + FRotator(0.f, randomRotations[rotationIndex], 0.f));

				if (!GetWorld()->LineTraceSingleByChannel(hit, nextLoc, nextLoc - GetActorUpVector() * 125.f, ECC_WorldStatic))
				{
					//goto redoMove; TODO: Infinite loop. Fix later.
				}
			}
		}

		SetActorLocation(FMath::VInterpConstantTo(GetActorLocation(), nextLoc, DeltaTime, moveSpeed));
	}
}
