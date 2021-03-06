// Fill out your copyright notice in the Description page of Project Settings.

#include "GravityTunnel.h"
#include "Components/BoxComponent.h"
#include "Mecha.h"

AGravityTunnel::AGravityTunnel()
{
	PrimaryActorTick.bCanEverTick = true;
}


void AGravityTunnel::BeginPlay()
{
	Super::BeginPlay();

	UBoxComponent* box;
	box = FindComponentByClass<UBoxComponent>();
	ensure(box);
	box->OnComponentBeginOverlap.AddDynamic(this, &AGravityTunnel::OnOverlapBegin);
}

void AGravityTunnel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGravityTunnel::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//TODO: OnOverlap wasn't catching non-player actors. Couldn't move pushable cubes into tunnel for example.

	AMecha* player = Cast<AMecha>(OtherActor);
	//AGridActor* grid = Cast<AGridActor>(OtherActor);

	if (player)
	{
		player->falling = false;
		FHitResult hit;
		FCollisionQueryParams params;
		params.AddIgnoredActor(player);
		params.AddIgnoredActor(this);

		if (GetWorld()->LineTraceSingleByChannel(hit, GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 1000.f, ECC_WorldStatic, params))
		{
			player->nextLoc = hit.ImpactPoint + hit.ImpactNormal * 50.f;
		}
	}
	/*else if(grid)
	{
		FHitResult hit;
		FCollisionQueryParams params;
		params.AddIgnoredActor(grid);

		if (GetWorld()->LineTraceSingleByChannel(hit, GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 1000.f, ECC_WorldStatic, params))
		{
			grid->nextLoc = hit.GetActor()->GetActorLocation() + hit.ImpactNormal * 100.f;
		}
	}*/
}
