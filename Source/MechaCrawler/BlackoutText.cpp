// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackoutText.h"
#include "GlobalTags.h"
#include "Kismet/GameplayStatics.h"
#include "Mecha.h"

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

	//Just for when player finds mecha first time
	if (this->Tags.Contains(Tags::MechaFirstTime))
	{
		APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		AMecha* player = Cast<AMecha>(playerPawn);
		player->nextLoc = (this->GetActorLocation());
		player->nextRot = (FQuat(this->GetActorRotation()));
		this->Destroy();
	}
}
