// Fill out your copyright notice in the Description page of Project Settings.

#include "MemoryAttune.h"
#include "Kismet/GameplayStatics.h"
#include "Mecha.h"
#include "LevelAudio.h"
#include "GlobalTags.h"
#include "Runtime/Engine/Public/TimerManager.h"

AMemoryAttune::AMemoryAttune()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AMemoryAttune::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMemoryAttune::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMemoryAttune::Use()
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
