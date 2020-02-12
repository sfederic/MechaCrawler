// Fill out your copyright notice in the Description page of Project Settings.

#include "Ship.h"
#include "Components/InputComponent.h"

AShip::AShip()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AShip::BeginPlay()
{
	Super::BeginPlay();
	
	//Init Widgets
	levelEntryWidget = CreateWidget<UEnterLevelWidget>(GetWorld(), levelEntryWidgetClass);
	check(levelEntryWidget);
}

void AShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	velocity = FMath::Clamp(velocity, velocityMin, velocityMax);
	reverseVelocity = FMath::Clamp(reverseVelocity, reverseVelocityMin, reverseVelocityMax);

	if (levelEntryWidget->IsInViewport() == false)
	{
		SetActorLocation(GetActorLocation() + (GetActorForwardVector() * velocity));
		SetActorLocation(GetActorLocation() + (GetActorForwardVector() * reverseVelocity));
	}
}

void AShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("Right", this, &AShip::RotateRight);
	InputComponent->BindAxis("Left", this, &AShip::RotateLeft);
	InputComponent->BindAxis("LeftMouseHeld", this, &AShip::Accelerate);
	InputComponent->BindAxis("RightMouseHeld", this, &AShip::Reverse);
}

void AShip::RotateRight(float val)
{
	if (levelEntryWidget->IsInViewport() == false)
	{
		if (val)
		{
			FRotator rot = GetActorRotation();
			rot.Yaw += rotateSpeed;
			SetActorRotation(rot);
		}
	}
}

void AShip::RotateLeft(float val)
{
	if (levelEntryWidget->IsInViewport() == false)
	{
		if (val)
		{
			FRotator rot = GetActorRotation();
			rot.Yaw -= rotateSpeed;
			SetActorRotation(rot);
		}
	}
}

void AShip::Accelerate(float val)
{
	if (levelEntryWidget->IsInViewport() == false)
	{
		if (val && bReversing == false)
		{
			bMovingForward = true;
			velocity += moveSpeed * val;
		}
		else
		{
			bMovingForward = false;
			velocity -= moveSpeed;
		}
	}
}

void AShip::Reverse(float val)
{
	if (levelEntryWidget->IsInViewport() == false)
	{
		if (val && bMovingForward == false)
		{
			bReversing = true;
			reverseVelocity -= reverseSpeed * val;
		}
		else
		{
			bReversing = false;
			reverseVelocity += reverseSpeed;
		}
	}
}
