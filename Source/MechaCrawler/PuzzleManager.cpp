// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzleManager.h"
#include "Activate.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Mecha.h"

APuzzleManager::APuzzleManager()
{
	PrimaryActorTick.bCanEverTick = true;

}

void APuzzleManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void APuzzleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (int i = 0; i < puzzleItems.Num(); i++)
	{
		if (puzzleItems[i]->bActivated == false)
		{
			return;
		}
	}

	if (!bActivated)
	{
		bActivated = true;
	}

	if (bActivated)
	{
		if (useActor)
		{
			IActivate* activate = Cast<IActivate>(useActor);
			if (activate)
			{
				activate->Use();
			}

			if (puzzleCompleteShot)
			{
				UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
				APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
				UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->DisableInput(controller);

				ULevelSequencePlayer* sequence = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), puzzleCompleteShot, FMovieSceneSequencePlaybackSettings(), sequenceActor);
				sequence->Play();
				sequence->OnStop.AddDynamic(this, &APuzzleManager::OnCinematicStop);
			}

			this->SetActorTickEnabled(false);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Use Actor not set on %s\n"), *GetNameSafe(this));
		}
	}
}

void APuzzleManager::OnCinematicStop()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	playerPawn->EnableInput(controller);

	AMecha* player = Cast<AMecha>(playerPawn);
	player->shootingWidget->AddToViewport(); //Need to add back in reticle HUD after RemoveAllWidgets()
}
