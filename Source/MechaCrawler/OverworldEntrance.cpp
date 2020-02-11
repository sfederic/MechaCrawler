// Fill out your copyright notice in the Description page of Project Settings.

#include "OverworldEntrance.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Ship.h"

AOverworldEntrance::AOverworldEntrance()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AOverworldEntrance::BeginPlay()
{
	Super::BeginPlay();
	
	box = FindComponentByClass<UBoxComponent>();
	box->OnComponentBeginOverlap.AddDynamic(this, &AOverworldEntrance::LoadLevel);
}

void AOverworldEntrance::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOverworldEntrance::LoadLevel(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AShip* playerShip = Cast<AShip>(OtherActor);
	if(playerShip)
	{ 
		playerShip->levelEntryWidget->levelName = this->levelName;
		playerShip->levelEntryWidget->AddToViewport();
		playerShip->velocity = 0.f;
		playerShip->reverseVelocity = 0.f;
		
		APlayerController* shipController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		shipController->SetInputMode(FInputModeUIOnly());
		shipController->bShowMouseCursor = true;
	}
}
