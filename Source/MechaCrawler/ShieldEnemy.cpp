// Fill out your copyright notice in the Description page of Project Settings.

#include "ShieldEnemy.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h" 
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "GlobalTags.h"

AShieldEnemy::AShieldEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AShieldEnemy::BeginPlay()
{
	Super::BeginPlay();

	player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void AShieldEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!this->Tags.Contains(Tags::Destroy))
	{
		FRotator rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), player->GetActorLocation());
		SetActorRotation(FMath::RInterpConstantTo(GetActorRotation(), rot, DeltaTime, rotateSpeed));
	}
}
