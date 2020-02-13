// Fill out your copyright notice in the Description page of Project Settings.

#include "Shop.h"
#include "Kismet/GameplayStatics.h"

AShop::AShop()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AShop::BeginPlay()
{
	Super::BeginPlay();
	
	shopWidget = CreateWidget<UUserWidget>(GetWorld(), shopWidgetClass);
}

void AShop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShop::Use()
{
	if (!shopWidget->IsInViewport())
	{
		shopWidget->AddToViewport();
		APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		controller->SetInputMode(FInputModeUIOnly());
		controller->bShowMouseCursor = true;
	}
}
