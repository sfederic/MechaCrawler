// Fill out your copyright notice in the Description page of Project Settings.

#include "IceComponent.h"
#include "DestructibleComponent.h"

UIceComponent::UIceComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	iceScale = FVector(1.f);
	waterScale = FVector(0.9f);
}

void UIceComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UIceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!isIce)
	{
		GetOwner()->SetActorScale3D(FMath::VInterpConstantTo(GetOwner()->GetActorScale(), waterScale, DeltaTime, meltSpeed));
	}
	else if (isIce)
	{
		GetOwner()->SetActorScale3D(FMath::VInterpConstantTo(GetOwner()->GetActorScale(), iceScale, DeltaTime, meltSpeed));
	}
}

void UIceComponent::TurnToWater()
{
	UDestructibleComponent* dc = GetOwner()->FindComponentByClass<UDestructibleComponent>();
	if (dc)
	{
		dc->SetMaterial(0, waterMaterial);
		GetOwner()->SetActorEnableCollision(false);
		isIce = false;
	}
}

void UIceComponent::TurnToIce()
{
	UDestructibleComponent* dc = GetOwner()->FindComponentByClass<UDestructibleComponent>();
	if (dc)
	{
		dc->SetMaterial(0, iceMaterial);
		GetOwner()->SetActorEnableCollision(true);
		isIce = true;
	}
}
