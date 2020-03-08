// Fill out your copyright notice in the Description page of Project Settings.

#include "Door.h"
#include "Engine/World.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"

UArrowComponent* arrow;

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();

	currentLoc = GetActorLocation();
	nextLoc = currentLoc;
	openState = false;

	arrow = FindComponentByClass<UArrowComponent>();
	if (arrow)
	{
		arrow->SetWorldRotation(openDirection.Rotation());
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Debug arrow not set %s"), *GetNameSafe(this));
	}

}

void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	currentLoc = FMath::VInterpConstantTo(currentLoc, nextLoc, DeltaTime, openSpeed);
	SetActorLocation(currentLoc);
}

void ADoor::Use()
{
	FHitResult moveHit;
	FCollisionQueryParams moveParams;
	moveParams.AddIgnoredActor(this);

	if (openState == false && currentLoc == nextLoc)
	{
		if (GetWorld()->LineTraceSingleByChannel(moveHit, GetActorLocation(),
			GetActorLocation() + openDirection * openDistance, ECC_WorldStatic, moveParams))
		{
			return;
		}

		nextLoc += openDirection * openDistance;
		arrow->SetWorldRotation(openDirection.Rotation().GetInverse());
		openState = true;

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), openSound, GetActorLocation());
	}
	else if (openState == true && currentLoc == nextLoc)
	{
		if (GetWorld()->LineTraceSingleByChannel(moveHit, GetActorLocation(),
			GetActorLocation() - openDirection * openDistance, ECC_WorldStatic, moveParams))
		{
			return;
		}

		nextLoc -= openDirection * openDistance;
		arrow->SetWorldRotation(openDirection.Rotation());
		openState = false;

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), closeSound, GetActorLocation());
	}


	//TODO: Was for elevator like functionality

	/*if (GetWorld()->LineTraceSingleByChannel(moveHit, GetActorLocation(),
		GetActorLocation() + openDirection * openDistance, ECC_WorldStatic, moveParams))
	{
		AGridActor* actor = Cast<AGridActor>(moveHit.GetActor());
		if (actor)
		{
			actor->nextLoc = nextLoc;
		}
	}*/
}

UScanData* ADoor::Scan()
{
	return FindComponentByClass<UScanData>();
}

void ADoor::Rebuild()
{
	this->currentLoc = this->originalLoc;
	this->nextLoc = this->originalLoc;
	this->openState = false;
}
