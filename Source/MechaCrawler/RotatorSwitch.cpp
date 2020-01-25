// Fill out your copyright notice in the Description page of Project Settings.

#include "RotatorSwitch.h"
#include "Mecha.h"
#include "Kismet/GameplayStatics.h"

ARotatorSwitch::ARotatorSwitch()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ARotatorSwitch::BeginPlay()
{
	Super::BeginPlay();
	
	APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	player = Cast<AMecha>(playerPawn);
}

void ARotatorSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bRotating)
	{
		player->canMove = false;

		for (int i = 0; i < actorsToRotate.Num(); i++)
		{
			if (actorsToRotate[i]->GetActorRotation().Equals(rotateAxis) == false)
			{
				actorsToRotate[i]->SetActorRotation(FMath::RInterpConstantTo(actorsToRotate[i]->GetActorRotation(), rotateAxis, DeltaTime, rotateSpeed));
			}
			else if (i == (actorsToRotate.Num() - 1))
			{
				if (actorsToRotate[i]->GetActorRotation().Equals(rotateAxis))
				{
					bRotating = false;
					player->canMove = true;
					rotateAxis = previousRotation;
				}
			}
		}
	}
}

void ARotatorSwitch::Use()
{
	if (bRotating == false)
	{
		bRotating = true;
		previousRotation = actorsToRotate[0]->GetActorRotation(); //TODO: not going to work with multiple different rots
	}
}
