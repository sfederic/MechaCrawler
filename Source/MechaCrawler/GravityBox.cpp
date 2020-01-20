// Fill out your copyright notice in the Description page of Project Settings.

#include "GravityBox.h"
#include "Mecha.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"

AGravityBox::AGravityBox()
{
	PrimaryActorTick.bCanEverTick = true;

	cableParams.AddIgnoredActor(this);

	originalGravityDirection = gravityDirection;
}

void AGravityBox::BeginPlay()
{
	Super::BeginPlay();
	
	currentLoc = GetActorLocation();
	nextLoc = currentLoc;

	boxCollision = FindComponentByClass<UBoxComponent>();
	boxCollision->OnComponentBeginOverlap.AddDynamic(this, &AGravityBox::OnPlayerOverlapBegin);

	cable = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), particleTemplate, GetActorLocation() + (gravityDirection * 50.f));
	cable->SetRelativeRotation(gravityDirection.Rotation());

	//This was a fucking bitch to figure out. Source-Method needs to be set to User-Set in particle emitter modules. vector params can also be used
	cable->SetBeamSourcePoint(0, GetActorLocation() + (gravityDirection * 50.f), 0);

	FHitResult cableHit;
	if (GetWorld()->LineTraceSingleByChannel(cableHit, GetActorLocation(), GetActorLocation() - (gravityDirection * 10000.f), ECC_WorldStatic, cableParams))
	{
		cable->SetBeamTargetPoint(0, cableHit.ImpactPoint, 0);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s cable particle has no end point."), *this->GetName());
		cable->SetBeamTargetPoint(0, FVector(0.f), 0);
	}
}

void AGravityBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	currentLoc = GetActorLocation();

	if (currentLoc.Equals(nextLoc))
	{
		const float fallDistance = 100.f;
		FHitResult fallHit;
		if (!GetWorld()->LineTraceSingleByChannel(fallHit, GetActorLocation(), GetActorLocation() + (gravityDirection * fallDistance), ECC_WorldStatic, cableParams))
		{
			nextLoc += gravityDirection * fallDistance;
		}
	}

	SetActorLocation(FMath::VInterpConstantTo(currentLoc, nextLoc, DeltaTime, fallSpeed));
}

void AGravityBox::Use()
{
	if (currentLoc.Equals(nextLoc))
	{
		gravityDirection = -gravityDirection;
	}
}

void AGravityBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//TODO: Make sure other actors that can overlap have this functionality (maybe put into GridActor). Actor needs a collision primitive
	AMecha* player = Cast<AMecha>(OtherActor);
	if (player)
	{
		if (player->lastLoc.Equals(nextLoc))
		{
			player->currentLoc = player->nextLoc;
			player->nextLoc = player->currentLoc;
		}
		else
		{
			player->currentLoc = player->lastLoc;
			player->nextLoc = player->lastLoc;
		}
	}
}

void AGravityBox::Rebuild()
{
	currentLoc = originalLoc;
	nextLoc = originalLoc;
	gravityDirection = originalGravityDirection;
}
