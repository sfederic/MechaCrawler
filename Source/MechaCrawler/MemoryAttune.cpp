// Fill out your copyright notice in the Description page of Project Settings.

#include "MemoryAttune.h"
#include "Kismet/GameplayStatics.h"
#include "Mecha.h"
#include "LevelAudio.h"
#include "GlobalTags.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "GameFramework/RotatingMovementComponent.h"

AMemoryAttune::AMemoryAttune()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AMemoryAttune::BeginPlay()
{
	Super::BeginPlay();
	
	if (!bActivated)
	{
		Tags.Remove(Tags::Useable);
	}
	else 
	{
		Tags.Add(Tags::Useable);
	}
}

void AMemoryAttune::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bActivated && activateIncrement < 1.0f)
	{
		activateIncrement += FApp::GetDeltaTime() * activateSpeed;
		FindComponentByClass<UMeshComponent>()->SetScalarParameterValueOnMaterials(TEXT("Blend"), activateIncrement);
	}
}

void AMemoryAttune::Use()
{
	if (bActivated == true)
	{
		APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		AMecha* player = Cast<AMecha>(playerPawn);

		player->memories.Remove(memoryName);
		player->memories.Add(memoryName);

		TArray<AActor*> levelSong;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALevelAudio::StaticClass(), levelSong);
		for (AActor* song : levelSong)
		{
			ALevelAudio* audio = Cast<ALevelAudio>(song);
			audio->song->FadeOut(1.0f, 0.f);
		}

		UGameplayStatics::PlaySound2D(GetWorld(), attuneSound, 1.5f, 0.85f);
		attuneWidget = CreateWidget<UUserWidget>(GetWorld(), attuneWidgetClass);
		attuneWidget->AddToViewport();

		FTimerHandle timerHandle;
		GetWorldTimerManager().SetTimer(timerHandle, this, &AMemoryAttune::UnpauseLevelSong, 6.0f, false);

		UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *GETENUMSTRING("EMemories", memoryName));

		Tags.Remove(Tags::Useable);

	}
	else
	{
		bActivated = true;
		Tags.Add(Tags::Useable);		
		FindComponentByClass<URotatingMovementComponent>()->RotationRate.Yaw = 3.0f;
	}
}

void AMemoryAttune::UnpauseLevelSong()
{
	TArray<AActor*> levelSong;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALevelAudio::StaticClass(), levelSong);
	for (AActor* song : levelSong)
	{
		ALevelAudio* audio = Cast<ALevelAudio>(song);
		audio->song->FadeIn(4.0f, 1.0f);
	}
}
