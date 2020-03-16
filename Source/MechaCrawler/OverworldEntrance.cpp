// Fill out your copyright notice in the Description page of Project Settings.

#include "OverworldEntrance.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Ship.h"
#include "Mecha.h"
#include "Components/StaticMeshComponent.h"
#include "GlobalTags.h"

AOverworldEntrance::AOverworldEntrance()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AOverworldEntrance::BeginPlay()
{
	Super::BeginPlay();
	
	Tags.Add(Tags::Tagable);

	box = FindComponentByClass<UBoxComponent>();
	box->OnComponentBeginOverlap.AddDynamic(this, &AOverworldEntrance::LoadLevel);

	//Get Original materials
	mesh = FindComponentByClass<UStaticMeshComponent>();
	originalMaterials = mesh->GetMaterials();

	player = Cast<AShip>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

void AOverworldEntrance::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bLocked)
	{
		for (int i = 0; i < player->memories.Num(); i++)
		{
			if (memoryToUnlockLevel == player->memories[i])
			{
				for (int j = 0; j < originalMaterials.Num(); j++)
				{
					mesh->SetMaterial(i, originalMaterials[i]);
				}

				bLocked = false;
				return;
			}
		}

		for (int i = 0; i < originalMaterials.Num(); i++)
		{
			mesh->SetMaterial(i, lockedMaterial);
		}
	}
}

void AOverworldEntrance::LoadLevel(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bLocked == false)
	{
		AShip* playerShip = Cast<AShip>(OtherActor);
		AMecha* playerMecha = Cast<AMecha>(OtherActor);
		if (playerShip)
		{
			playerShip->levelEntryWidget->levelName = this->levelName;
			playerShip->levelEntryWidget->AddToViewport();
			playerShip->velocity = 0.f;
			playerShip->reverseVelocity = 0.f;

			APlayerController* shipController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			shipController->SetInputMode(FInputModeUIOnly());
			shipController->bShowMouseCursor = true;
		}
		else if (playerMecha)
		{
			playerMecha->levelExitWidget->levelName = this->levelName;
			playerMecha->levelExitWidget->AddToViewport();
			playerMecha->canMove = false;
			//playerMecha->currentLoc = GetActorLocation();
			//playerMecha->nextLoc = GetActorLocation();

			APlayerController* mechController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			mechController->SetInputMode(FInputModeUIOnly());
			mechController->bShowMouseCursor = true;
		}
	}
}
