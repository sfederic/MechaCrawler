// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzleBeam.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Activate.h"
#include "PuzzleBeamInteract.h"
#include "Components/StaticMeshComponent.h"

APuzzleBeam::APuzzleBeam()
{
	PrimaryActorTick.bCanEverTick = true;

	beamParams.AddIgnoredActor(this);
}

void APuzzleBeam::BeginPlay()
{
	Super::BeginPlay();
	
	beam = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), beamTemplate, GetActorLocation(), FRotator(), true);
}

void APuzzleBeam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	beam->SetRelativeRotation(GetActorForwardVector().Rotation());
	beam->SetBeamSourcePoint(0, GetActorLocation(), 0);

	if (GetWorld()->LineTraceSingleByChannel(beamHit, GetActorLocation(), GetActorLocation() + (GetActorForwardVector() * beamDistance), ECC_WorldStatic, beamParams))
	{
		beam->SetBeamTargetPoint(0, beamHit.ImpactPoint, 0);

		AActor* beamHitActor = beamHit.GetActor();
		if (beamHitActor->IsA<APuzzleBeamInteract>())
		{
			APuzzleBeamInteract* interact = Cast<APuzzleBeamInteract>(beamHitActor);
			interact->bOn = true;
			previousActor = interact;
		}
	}
	else
	{
		beam->SetBeamTargetPoint(0, GetActorLocation() + (GetActorForwardVector() * beamDistance), 0);

		if (previousActor != nullptr)
		{
			previousActor->bOn = false;
		}
	}
}

void APuzzleBeam::Rebuild()
{
	SetActorRotation(originalRot);
}
