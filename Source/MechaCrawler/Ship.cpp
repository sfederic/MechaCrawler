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
	
}

void AShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	velocity = FMath::Clamp(velocity, velocityMin, velocityMax);
	reverseVelocity = FMath::Clamp(reverseVelocity, reverseVelocityMin, reverseVelocityMax);

	if (!bReversing)
	{
		SetActorLocation(GetActorLocation() + (GetActorForwardVector() * velocity));
	}
	else
	{
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
	if (val)
	{
		FRotator rot = GetActorRotation();
		rot.Yaw += rotateSpeed;
		SetActorRotation(rot);
	}
}

void AShip::RotateLeft(float val)
{
	if (val)
	{
		FRotator rot = GetActorRotation();
		rot.Yaw -= rotateSpeed;
		SetActorRotation(rot);
	}
}

void AShip::Accelerate(float val)
{
	if (val)
	{
		velocity += moveSpeed * val;
	}
	else 
	{
		velocity -= moveSpeed;
	}
}

void AShip::Reverse(float val)
{
	if (val)
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
