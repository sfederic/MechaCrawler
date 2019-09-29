// Fill out your copyright notice in the Description page of Project Settings.

#include "Mecha.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h"
#include "DestructibleComponent.h"

UCameraComponent* camera;

FHitResult moveHit;
FCollisionQueryParams moveParams;

FVector rootAxes[4];

float moveDistance = 100.f;
float traceDistance = 100.f;

bool falling = false;

AMecha::AMecha()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics; //Seems to fix fall through floor effect (for now)
	moveParams.AddIgnoredActor(this);
}

void AMecha::BeginPlay()
{
	Super::BeginPlay();
	
	rootAxes[0] = RootComponent->GetForwardVector();
	rootAxes[1] = -RootComponent->GetForwardVector();
	rootAxes[2] = RootComponent->GetRightVector();
	rootAxes[3] = -RootComponent->GetRightVector();
	
	currentLoc = GetActorLocation();
	nextLoc = currentLoc;

	currentRot = FQuat(GetActorRotation());
	nextRot = currentRot;

	camera = FindComponentByClass<UCameraComponent>();
	check(camera);
	cameraRot = camera->GetComponentRotation();
}

void AMecha::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	rootAxes[0] = RootComponent->GetForwardVector();
	rootAxes[1] = -RootComponent->GetForwardVector();
	rootAxes[2] = RootComponent->GetRightVector();
	rootAxes[3] = -RootComponent->GetRightVector();

	float forwardAngle = INFINITY;
	float rightAngle = INFINITY;

	int forwardAxisIndex = 0;
	int rightAxisIndex = 0;

	for (int i = 0; i < 4; i++)
	{
		float tempForwardAngle = FMath::Acos(FVector::DotProduct(camera->GetForwardVector(), rootAxes[i]));
		if (tempForwardAngle < forwardAngle)
		{
			forwardAngle = tempForwardAngle;
			forwardAxisIndex = i;
		}

		float tempRightAngle = FMath::Acos(FVector::DotProduct(camera->GetRightVector(), rootAxes[i]));
		if (tempRightAngle < rightAngle)
		{
			rightAngle = tempRightAngle;
			rightAxisIndex = i;
		}
	}

	forwardAxis = rootAxes[forwardAxisIndex];
	rightAxis = rootAxes[rightAxisIndex];

	//Gravity
	if (currentLoc == nextLoc && currentRot == nextRot)
	{
		FVector loc = GetActorLocation();

		if (!GetWorld()->LineTraceSingleByChannel(moveHit, loc, loc - (RootComponent->GetUpVector() * traceDistance), ECC_WorldStatic, moveParams))
		{
			falling = true;

			nextLoc = loc - (RootComponent->GetUpVector() * moveDistance);
			nextLoc.X = FMath::RoundToFloat(nextLoc.X);
			nextLoc.Y = FMath::RoundToFloat(nextLoc.Y);
			nextLoc.Z = FMath::RoundToFloat(nextLoc.Z);
		}
		else
		{
			falling = false;
		}
	}

	///MOVE'N
	APlayerController* controller = (APlayerController*)GetController();

	if(controller->IsInputKeyDown(EKeys::W))
	{
		MoveForward(1.0f);
	}
	if (controller->IsInputKeyDown(EKeys::S))
	{
		MoveBack(1.0f);
	}
	if (controller->IsInputKeyDown(EKeys::A))
	{
		MoveLeft(1.0f);
	}
	if (controller->IsInputKeyDown(EKeys::D))
	{
		MoveRight(1.0f);
	}

	//SHOOT'N
	if (controller->IsInputKeyDown(EKeys::LeftMouseButton)) 
	{
		FHitResult shootHit;
		if (GetWorld()->LineTraceSingleByChannel(shootHit, GetActorLocation(), GetActorLocation() + camera->GetForwardVector() * 1000.f,
			ECC_Destructible))
		{
			UDestructibleComponent* dc = Cast<UDestructibleComponent>(shootHit.GetComponent());
			if (dc)
			{
				dc->ApplyDamage(damageAmount, shootHit.ImpactPoint, camera->GetForwardVector(), damageStrength);
				dc->GetOwner()->SetLifeSpan(1.f);
				dc->GetOwner()->Tags.Add("Destroy");
			}
		}
	}

	//Actor/Camera Rotation & Movement
	camera->SetRelativeRotation(cameraRot);

	currentRot = FMath::QInterpConstantTo(currentRot, nextRot, DeltaTime, rotateSpeed);
	RootComponent->SetRelativeRotation(currentRot);

	currentLoc = FMath::VInterpConstantTo(currentLoc, nextLoc, DeltaTime, moveSpeed);
	SetActorLocation(currentLoc);
}

void AMecha::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/*InputComponent->BindAxis("Forward", this, &AMecha::MoveForward);
	InputComponent->BindAxis("Back", this, &AMecha::MoveBack);
	InputComponent->BindAxis("Left", this, &AMecha::MoveLeft);
	InputComponent->BindAxis("Right", this, &AMecha::MoveRight);*/
	InputComponent->BindAxis("Mouse X", this, &AMecha::LookYaw);
	InputComponent->BindAxis("Mouse Y", this, &AMecha::LookPitch);
}

void AMecha::MoveForward(float val)
{
	if (val != 0.f && falling == false)
	{
		FVector loc = GetActorLocation();

		if (currentLoc == nextLoc && currentRot == nextRot)
		{
			//TODO: move traces look like shit, find a way to ignore channels/set channels
			TArray<FHitResult> results;
			if (GetWorld()->LineTraceMultiByChannel(results, loc, loc + (forwardAxis * traceDistance), ECC_WorldStatic, moveParams))
			{
				for (int i = 0; i < results.Num(); i++)
				{
					if (!results[i].GetActor()->Tags.Contains("Destroy"))
					{
						goto move;
					}
				}

				return;

				move:

				if (rightAxis.Equals(RootComponent->GetRightVector()))
				{
					nextRot *= FQuat(FVector::RightVector, FMath::DegreesToRadians(-90.f));
				}
				else if (rightAxis.Equals(RootComponent->GetForwardVector()))
				{
					nextRot *= FQuat(FVector::ForwardVector, FMath::DegreesToRadians(-90.f));
				}
				else if (rightAxis.Equals(-RootComponent->GetRightVector()))
				{
					nextRot *= FQuat(FVector::RightVector, FMath::DegreesToRadians(90.f));
				}
				else if (rightAxis.Equals(-RootComponent->GetForwardVector()))
				{
					nextRot *= FQuat(FVector::ForwardVector, FMath::DegreesToRadians(90.f));
				}

				nextRot.Normalize();
			}
			else
			{
				nextLoc = loc + (forwardAxis * moveDistance);
				nextLoc.X = FMath::RoundToFloat(nextLoc.X);
				nextLoc.Y = FMath::RoundToFloat(nextLoc.Y);
				nextLoc.Z = FMath::RoundToFloat(nextLoc.Z);
			}
		}
	}
}

void AMecha::MoveBack(float val)
{
	if (val != 0.f && falling == false)
	{
		FVector loc = GetActorLocation();

		if (currentLoc == nextLoc && currentRot == nextRot)
		{
			TArray<FHitResult> results;
			if (GetWorld()->LineTraceMultiByChannel(results, loc, loc - (forwardAxis * traceDistance), ECC_WorldStatic, moveParams))
			{
				for (int i = 0; i < results.Num(); i++)
				{
					if (!results[i].GetActor()->Tags.Contains("Destroy"))
					{
						goto move;
					}
				}

				return;

				move:

				if (rightAxis.Equals(RootComponent->GetRightVector()))
				{
					nextRot *= FQuat(FVector::RightVector, FMath::DegreesToRadians(90.f));
				}
				else if (rightAxis.Equals(RootComponent->GetForwardVector()))
				{
					nextRot *= FQuat(FVector::ForwardVector, FMath::DegreesToRadians(90.f));
				}
				else if (rightAxis.Equals(-RootComponent->GetRightVector()))
				{
					nextRot *= FQuat(FVector::RightVector, FMath::DegreesToRadians(-90.f));
				}
				else if (rightAxis.Equals(-RootComponent->GetForwardVector()))
				{
					nextRot *= FQuat(FVector::ForwardVector, FMath::DegreesToRadians(-90.f));
				}

				nextRot.Normalize();
			}
			else
			{
				nextLoc = loc - (forwardAxis * moveDistance);
				nextLoc.X = FMath::RoundToFloat(nextLoc.X);
				nextLoc.Y = FMath::RoundToFloat(nextLoc.Y);
				nextLoc.Z = FMath::RoundToFloat(nextLoc.Z);
			}
		}
	}
}

void AMecha::MoveLeft(float val)
{
	if (val != 0.f && falling == false)
	{
		FVector loc = GetActorLocation();

		if (currentLoc == nextLoc && currentRot == nextRot)
		{
			TArray<FHitResult> results;
			if (GetWorld()->LineTraceMultiByChannel(results, loc, loc - (rightAxis * traceDistance), ECC_WorldStatic, moveParams))
			{
				for (int i = 0; i < results.Num(); i++)
				{
					if (!results[i].GetActor()->Tags.Contains("Destroy"))
					{
						goto move;
					}
				}

				return;

				move:

				if (forwardAxis.Equals(RootComponent->GetRightVector()))
				{
					nextRot *= FQuat(FVector::RightVector, FMath::DegreesToRadians(-90.f));
				}
				else if (forwardAxis.Equals(RootComponent->GetForwardVector()))
				{
					nextRot *= FQuat(FVector::ForwardVector, FMath::DegreesToRadians(-90.f));
				}
				else if (forwardAxis.Equals(-RootComponent->GetRightVector()))
				{
					nextRot *= FQuat(FVector::RightVector, FMath::DegreesToRadians(90.f));
				}
				else if (forwardAxis.Equals(-RootComponent->GetForwardVector()))
				{
					nextRot *= FQuat(FVector::ForwardVector, FMath::DegreesToRadians(90.f));
				}

				nextRot.Normalize();
			}
			else
			{
				nextLoc = loc - (rightAxis * moveDistance);
				nextLoc.X = FMath::RoundToFloat(nextLoc.X);
				nextLoc.Y = FMath::RoundToFloat(nextLoc.Y);
				nextLoc.Z = FMath::RoundToFloat(nextLoc.Z);
			}
		}
	}
}

void AMecha::MoveRight(float val)
{
	if (val != 0.f && falling == false)
	{
		FVector loc = GetActorLocation();

		if (currentLoc == nextLoc && currentRot == nextRot)
		{
			TArray<FHitResult> results;
			if (GetWorld()->LineTraceMultiByChannel(results, loc, loc + (rightAxis * traceDistance), ECC_WorldStatic, moveParams))
			{
				for (int i = 0; i < results.Num(); i++)
				{
					if (!results[i].GetActor()->Tags.Contains("Destroy"))
					{
						goto move;
					}
				}

				return;

			move:

				if (forwardAxis.Equals(RootComponent->GetRightVector()))
				{
					nextRot *= FQuat(FVector::RightVector, FMath::DegreesToRadians(90.f));
				}
				else if (forwardAxis.Equals(RootComponent->GetForwardVector()))
				{
					nextRot *= FQuat(FVector::ForwardVector, FMath::DegreesToRadians(90.f));
				}
				else if (forwardAxis.Equals(-RootComponent->GetRightVector()))
				{
					nextRot *= FQuat(FVector::RightVector, FMath::DegreesToRadians(-90.f));
				}
				else if (forwardAxis.Equals(-RootComponent->GetForwardVector()))
				{
					nextRot *= FQuat(FVector::ForwardVector, FMath::DegreesToRadians(-90.f));
				}

				nextRot.Normalize();
			}
			else
			{
				nextLoc = loc + (rightAxis * moveDistance);
				nextLoc.X = FMath::RoundToFloat(nextLoc.X);
				nextLoc.Y = FMath::RoundToFloat(nextLoc.Y);
				nextLoc.Z = FMath::RoundToFloat(nextLoc.Z);
			}
		}
	}
}

void AMecha::LookYaw(float val)
{
	cameraRot.Yaw += cameraSpeed * val;
}

void AMecha::LookPitch(float val)
{
	cameraRot.Pitch -= cameraSpeed * val;
	cameraRot.Pitch = FMath::Clamp(cameraRot.Pitch, -70.f, 70.f);
}
