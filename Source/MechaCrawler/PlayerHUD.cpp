// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "Misc/App.h"

FString newText;

int textIndex;
double textScrollTimer;

void APlayerHUD::BeginPlay()
{
	text = FString(TEXT("Hello my name is HUD"));
}

void APlayerHUD::DrawHUD()
{
	Super::DrawHUD();

	if (textIndex < text.Len())
	{
		textScrollTimer += FApp::GetDeltaTime();

		if (textScrollTimer > 0.1f)
		{
			newText.AppendChar(text[textIndex]);

			textIndex++;
			textScrollTimer = 0.f;
		}
	}

	DrawText(newText, FLinearColor::Black, 10.f, 10.f, nullptr, 2.0f);
}
