// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackoutText.h"
#include "GlobalTags.h"
#include "Kismet/GameplayStatics.h"
#include "Mecha.h"
#include "Engine/World.h"
#include "Runtime/Engine/Public/TimerManager.h"

ABlackoutText::ABlackoutText()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ABlackoutText::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABlackoutText::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABlackoutText::Use()
{
	fadeWidget = CreateWidget<UFadeToBlackWidget>(GetWorld(), fadeWidgetClass);
	fadeWidget->text = fadeOutText;
	fadeWidget->AddToViewport();

	//Just for when player finds mecha first time edge case
	if (this->Tags.Contains(Tags::MechaFirstTime))
	{
		FTimerHandle timerHandle;
		GetWorldTimerManager().SetTimer(timerHandle, this, &ABlackoutText::MovePlayer, 1.5f, false);
	}
}

void ABlackoutText::MovePlayer()
{
	APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	AMecha* player = Cast<AMecha>(playerPawn);
	player->nextLoc = (this->GetActorLocation());
	player->nextRot = (FQuat(this->GetActorRotation()));
	this->Destroy();
}
