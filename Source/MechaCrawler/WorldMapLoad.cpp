// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldMapLoad.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "MainGameInstance.h"

AWorldMapLoad::AWorldMapLoad()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AWorldMapLoad::BeginPlay()
{
	Super::BeginPlay();
	
	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraFade(1.0f, 0.f, 5.0f, FColor::Black);
	UMainGameInstance* instance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	TArray<AActor*> spawnActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), instance->levelLoadTag, spawnActors);
	for (AActor* spawnPoint : spawnActors)
	{
		UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->SetActorLocation(spawnPoint->GetActorLocation()); //Only ever going to be one loop anyway
		UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->SetActorRotation(spawnPoint->GetActorRotation());
	}
}

void AWorldMapLoad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

