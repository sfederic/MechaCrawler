// Fill out your copyright notice in the Description page of Project Settings.

#include "CloudMoveComponent.h"
#include "Gameframework/Actor.h"
#include "Kismet/KismetMathLibrary.h"

UCloudMoveComponent::UCloudMoveComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCloudMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	cloud = GetOwner();
	scaleVector = cloud->GetActorScale3D();
	randomScaleVector = UKismetMathLibrary::RandomUnitVector() * 10.f;
	randomScaleVector.Z = 1.5;

	scale = (scaleVector.X + scaleVector.Y) / 2.0f;
}

void UCloudMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	cloud->SetActorLocation(cloud->GetActorLocation() + (moveVector * moveSpeed));

	scaleVector = FMath::VInterpTo(scaleVector, randomScaleVector, DeltaTime, interpScaleSpeed);
	if (scaleVector.Equals(randomScaleVector))
	{
		randomScaleVector = UKismetMathLibrary::RandomUnitVector() * scale; 
		randomScaleVector.Z = 1.5; //You want to keep the z axis small enough (unless you want tall clouds)
	}
	cloud->SetActorScale3D(scaleVector);
}

