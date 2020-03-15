// Fill out your copyright notice in the Description page of Project Settings.

#include "MemoryAttune.h"
#include "Kismet/GameplayStatics.h"
#include "Mecha.h"
#include "LevelAudio.h"
#include "GlobalTags.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "MainGameInstance.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

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
		if (bEndLevelOnTouch == false)
		{
			GetWorldTimerManager().SetTimer(timerHandle, this, &AMemoryAttune::UnpauseLevelSong, 6.0f, false);
		} 
		else if (bEndLevelOnTouch == true)
		{
			GetWorldTimerManager().SetTimer(timerHandle, this, &AMemoryAttune::ExitLevel, 6.0f, false);
		}

		UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *GETENUMSTRING("EMemories", memoryName));
		Tags.Remove(Tags::Useable);

	}
	else
	{
		bActivated = true;
		Tags.Add(Tags::Useable);		
		FindComponentByClass<URotatingMovementComponent>()->RotationRate.Yaw = 3.0f;
		FindComponentByClass<UParticleSystemComponent>()->Activate();
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

void AMemoryAttune::ExitLevel()
{
	//Just copy and pasted from ALevelChange instead of OverWorldEntrace widget. Don't have to deal with menu 'Yes/No' then
	UMainGameInstance* instance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (instance)
	{
		UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraFade(1.0f, 0.f, 5.0f, FColor::Black);
		instance->levelLoadTag = FName(*UGameplayStatics::GetCurrentLevelName(GetWorld())); //https://wiki.unrealengine.com/String_Conversions:_FString_to_FName,_FString_to_Int32,_Float_to_FString#Converting_FString_to_FNames

		TArray<UUserWidget*> widgetsToRemove;
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), widgetsToRemove, UUserWidget::StaticClass());
		for (UUserWidget* widget : widgetsToRemove)
		{
			widget->RemoveFromViewport();
		}

		UGameplayStatics::OpenLevel(GetWorld(), levelToExitToName);
	}
}
