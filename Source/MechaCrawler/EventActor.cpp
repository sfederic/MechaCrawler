// Fill out your copyright notice in the Description page of Project Settings.

#include "EventActor.h"
#include "Components/BoxComponent.h"
#include "LevelSequencePlayer.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Mecha.h"

AEventActor::AEventActor()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AEventActor::BeginPlay()
{
	Super::BeginPlay();

	box = FindComponentByClass<UBoxComponent>();
	box->OnComponentBeginOverlap.AddDynamic(this, &AEventActor::OnPlayerOverlap);

	if (bIsActivated)
	{
		box->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	else if (!bIsActivated)
	{
		box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AEventActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEventActor::Activate()
{
	bIsActivated = true;
	FindComponentByClass<UBoxComponent>()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AEventActor::OnPlayerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AMecha>())
	{
		UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
		APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		OtherActor->DisableInput(controller);

		if (shot)
		{
			ULevelSequencePlayer* sequence = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), shot, FMovieSceneSequencePlaybackSettings(), sequenceActor);
			sequence->Play();
			sequence->OnStop.AddDynamic(this, &AEventActor::OnCinematicStop);

			bIsActivated = false;
		}
	}
}

void AEventActor::OnCinematicStop()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	playerPawn->EnableInput(controller);

	AMecha* player = Cast<AMecha>(playerPawn);
	player->shootingWidget->AddToViewport(); //Need to add back in reticle HUD after RemoveAllWidgets()
}

