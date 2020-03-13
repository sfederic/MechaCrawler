// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelChange.h"
#include "Mecha.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "LevelSave.h"
#include "DialogueBox.h"
#include "MainGameInstance.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

ALevelChange::ALevelChange()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ALevelChange::BeginPlay()
{
	Super::BeginPlay();
	box = FindComponentByClass<UBoxComponent>();
	box->OnComponentBeginOverlap.AddDynamic(this, &ALevelChange::OnPlayerOverlap);
}

void ALevelChange::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelChange::OnPlayerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMecha* player = Cast<AMecha>(OtherActor);
	if (player)
	{
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

			UGameplayStatics::OpenLevel(GetWorld(), levelName);
		}
	}
}

