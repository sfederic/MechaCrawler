// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "Misc/App.h"
#include "Kismet/GameplayStatics.h" 

FString text;
FString newText;

int textIndex;

double textScrollTimer;
double endTextTimer;

void APlayerHUD::BeginPlay()
{
	//text = FString(TEXT("\"You did know you can shoot those bricks right?\""));
	//textIndex = 0;
	//newText.Reset();

	/*if (activateWidget)
	{
		activateWidget = CreateWidget<UActivateWidget>(GetWorld(), widgetClass);
		activateWidget->AddToViewport();
	}*/
}

void APlayerHUD::DrawHUD()
{
	Super::DrawHUD();
	
	/*if (textIndex < text.Len())
	{
		textScrollTimer += FApp::GetDeltaTime();

		if (textScrollTimer > 0.1f)
		{
			newText.AppendChar(text[textIndex]);

			textIndex++;
			textScrollTimer = 0.f;
		}
	}

	if (textIndex != newText.Len())
	{
		DrawText(newText, FLinearColor::Black, 10.f, 10.f, nullptr, 2.0f);
	}*/
}
