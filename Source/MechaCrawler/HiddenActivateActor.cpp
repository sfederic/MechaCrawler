// Fill out your copyright notice in the Description page of Project Settings.


#include "HiddenActivateActor.h"
#include "Components/MeshComponent.h"

AHiddenActivateActor::AHiddenActivateActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AHiddenActivateActor::BeginPlay()
{
	Super::BeginPlay();

	this->SetActorHiddenInGame(bIsHidden);
	this->SetActorEnableCollision(false);
	FindComponentByClass<UMeshComponent>()->SetScalarParameterValueOnMaterials(TEXT("FadeValue"), fadeIncrement);
}

void AHiddenActivateActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsHidden == false && fadeIncrement < 1.0f)
	{
		fadeIncrement += FApp::GetDeltaTime() * fadeSpeed;
		FindComponentByClass<UMeshComponent>()->SetScalarParameterValueOnMaterials(TEXT("FadeValue"), fadeIncrement);

		if (fadeIncrement >= 1.0f)
		{
			bIsHidden = true;
			fadeIncrement = 0.f;
		}
	}
}

void AHiddenActivateActor::Use()
{
	bIsHidden = false;
	SetActorHiddenInGame(bIsHidden);
	this->SetActorEnableCollision(true);
}