// Fill out your copyright notice in the Description page of Project Settings.

#include "Mecha.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h"
#include "DestructibleComponent.h"
#include "Activate.h"
#include "Scannable.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "GlobalTags.h"

UCameraComponent* camera;
APlayerController* controller;

AMecha::AMecha()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true; //Seems to fix fall through on play start when Mech starts on object
	PrimaryActorTick.TickGroup = TG_PrePhysics; //Seems to fix fall through floor effect. Other Actors needs the same

	moveParams.AddIgnoredActor(this);
}

void AMecha::BeginPlay()
{
	Super::BeginPlay();
	
	//INIT WIDGETS
	useWidget = CreateWidget<UActivateWidget>(GetWorld(), useWidgetClass);
	if (useWidget)
	{
		useWidget->useText = FString("RMB: Use");
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Use widget not set in Mecha.cpp"));
	}

	scanWidget = CreateWidget<UScanWidget>(GetWorld(), scanWidgetClass);
	if (scanWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Scan widget not set in Mecha.cpp"));
	}

	inventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), inventoryWidgetClass);
	if (inventoryWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory widget not set in Mecha.cpp"));
	}

	//WAYPOINT INIT
	wayPoint = FindComponentByClass<UParticleSystemComponent>();
	check(wayPoint);
	if (wayPoint)
	{
		wayPoint->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, false));
		wayPoint->SetVisibility(false);
	}

	previousMoveSpeed = moveSpeed;

	rootAxes[0] = RootComponent->GetForwardVector();
	rootAxes[1] = -RootComponent->GetForwardVector();
	rootAxes[2] = RootComponent->GetRightVector();
	rootAxes[3] = -RootComponent->GetRightVector();
	
	currentLoc = GetActorLocation();
	nextLoc = currentLoc;

	currentRot = FQuat(GetActorRotation());
	nextRot = currentRot;

	//INIT CAMERA
	camera = FindComponentByClass<UCameraComponent>();
	check(camera);
	cameraRot = camera->GetComponentRotation();

	controller = (APlayerController*)GetController();
}

void AMecha::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//MOVEMENT AXIS
	rootAxes[0] = RootComponent->GetForwardVector();
	rootAxes[1] = -RootComponent->GetForwardVector();
	rootAxes[2] = RootComponent->GetRightVector();
	rootAxes[3] = -RootComponent->GetRightVector();

	float forwardAngle = INFINITY;
	float rightAngle = INFINITY;

	forwardAxisIndex = 0;
	rightAxisIndex = 0;

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

	//GRAVITY
	if (currentLoc == nextLoc && currentRot == nextRot)
	{
		FVector loc = GetActorLocation();

		if (!GetWorld()->LineTraceSingleByChannel(moveHit, loc, loc - (RootComponent->GetUpVector() * traceDistance), ECC_WorldStatic, moveParams))
		{
			falling = true;

			//TODO: Acellerating fallspeed mucks with linetrace collision. Might add back in. Might take out. Feels good though.
			moveSpeed += FApp::GetDeltaTime() + 100.0f;

			nextLoc = loc - (RootComponent->GetUpVector() * moveDistance);
			nextLoc.X = FMath::RoundToFloat(nextLoc.X);
			nextLoc.Y = FMath::RoundToFloat(nextLoc.Y);
			nextLoc.Z = FMath::RoundToFloat(nextLoc.Z);
		}
		else
		{
			moveSpeed = previousMoveSpeed;
			falling = false;
		}
	}

	//SCANNING
	if (scanning && scanWidget)
	{
		if (GetWorld()->LineTraceSingleByChannel(scanHit, camera->GetComponentLocation(),
			camera->GetComponentLocation() + camera->GetForwardVector() * scanDistance, ECC_WorldStatic))
		{
			AActor* actor = scanHit.GetActor();
			{
				UScanData* scanData = actor->FindComponentByClass<UScanData>();
				if (scanData)
				{
					scanWidget->scanEntry = scanData->scanText;
					scanWidget->scanNameEntry = scanData->scanName;
				}
			}
		}
		else
		{
			scanWidget->scanEntry = FString(TEXT("No Scan data."));
			scanWidget->scanNameEntry = FString(TEXT("Scanning..."));
		}
	}

	//USE UI
	if (useWidget && scanning == false)
	{
		if (scanWidget)
		{
			if (scanWidget->IsInViewport())
			{
				scanWidget->RemoveFromViewport();
			}
		}

		if (GetWorld()->LineTraceSingleByChannel(lookHit, camera->GetComponentLocation(),
			camera->GetComponentLocation() + camera->GetForwardVector() * useDistance, ECC_WorldStatic))
		{
			if (lookHit.GetActor()->Tags.Contains(Tags::Useable))
			{
				if (useWidget->IsInViewport() == false)
				{
					if(lookHit.GetActor()->Tags.Contains(Tags::Pushable))
					{
						useWidget->useText = "Right Mouse: Push";
					}
					else if (lookHit.GetActor()->Tags.Contains(Tags::Pickup))
					{
						useWidget->useText = "Right Mouse: Pickup";
					}

					useWidget->AddToViewport();
				}
			}
			else if (useWidget->IsInViewport() == true)
			{
				useWidget->RemoveFromViewport();
				useWidget->useText = "RMB: Use";
			}
		}
		else
		{
			if (useWidget->IsInViewport() == true)
			{
				useWidget->RemoveFromViewport();
				useWidget->useText = "RMB: Use";
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

	InputComponent->BindAxis("Forward", this, &AMecha::MoveForward);
	InputComponent->BindAxis("Back", this, &AMecha::MoveBack);
	InputComponent->BindAxis("Left", this, &AMecha::MoveLeft);
	InputComponent->BindAxis("Right", this, &AMecha::MoveRight);
	InputComponent->BindAxis("Mouse X", this, &AMecha::LookYaw);
	InputComponent->BindAxis("Mouse Y", this, &AMecha::LookPitch);
	
	InputComponent->BindAction("Scan", EInputEvent::IE_Pressed, this, &AMecha::SetScan);
	InputComponent->BindAction("RightMouse", EInputEvent::IE_Pressed, this, &AMecha::RightMousePressed);
	InputComponent->BindAction("LeftMouse", EInputEvent::IE_Pressed, this, &AMecha::LeftMousePressed);
	InputComponent->BindAction("Space", EInputEvent::IE_Pressed, this, &AMecha::SetWayPoint);
	InputComponent->BindAction("Enter", EInputEvent::IE_Pressed, this, &AMecha::OpenInventory);
}

void AMecha::MoveForward(float val)
{
	if (val != 0.f && falling == false)
	{
		FVector loc = GetActorLocation();

		if (currentLoc == nextLoc && currentRot == nextRot)
		{
			//TODO: move traces look like shit, find a way to ignore channels/set channels (Am I talking about destruction here? Who is this guy?)
			TArray<FHitResult> results;
			if (GetWorld()->LineTraceMultiByChannel(results, loc, loc + (forwardAxis * traceDistance), ECC_WorldStatic, moveParams))
			{
				for (int i = 0; i < results.Num(); i++)
				{
					if (!results[i].GetActor()->Tags.Contains(Tags::Destroy) || !results[i].GetActor()->Tags.Contains(Tags::Pickup))
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
				FVector previousLoc = nextLoc;

				nextLoc = loc + (forwardAxis * moveDistance);
				nextLoc.X = FMath::RoundToFloat(nextLoc.X);
				nextLoc.Y = FMath::RoundToFloat(nextLoc.Y);
				nextLoc.Z = FMath::RoundToFloat(nextLoc.Z);

				if (!GetWorld()->LineTraceSingleByChannel(moveHit, nextLoc, nextLoc - GetActorUpVector() * maxFallDistance, ECC_WorldStatic, moveParams))
				{
					nextLoc = previousLoc;
					return;
				}
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
					if (!results[i].GetActor()->Tags.Contains(Tags::Destroy) || !results[i].GetActor()->Tags.Contains(Tags::Pickup))
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
				FVector previousLoc = nextLoc;

				nextLoc = loc - (forwardAxis * moveDistance);
				nextLoc.X = FMath::RoundToFloat(nextLoc.X);
				nextLoc.Y = FMath::RoundToFloat(nextLoc.Y);
				nextLoc.Z = FMath::RoundToFloat(nextLoc.Z);

				if (!GetWorld()->LineTraceSingleByChannel(moveHit, nextLoc, nextLoc - GetActorUpVector() * maxFallDistance, ECC_WorldStatic, moveParams))
				{
					nextLoc = previousLoc;
					return;
				}
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
					if (!results[i].GetActor()->Tags.Contains(Tags::Destroy) || !results[i].GetActor()->Tags.Contains(Tags::Pickup))
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
				FVector previousLoc = nextLoc;

				nextLoc = loc - (rightAxis * moveDistance);
				nextLoc.X = FMath::RoundToFloat(nextLoc.X);
				nextLoc.Y = FMath::RoundToFloat(nextLoc.Y);
				nextLoc.Z = FMath::RoundToFloat(nextLoc.Z);

				if (!GetWorld()->LineTraceSingleByChannel(moveHit, nextLoc, nextLoc - GetActorUpVector() * maxFallDistance, ECC_WorldStatic, moveParams))
				{
					nextLoc = previousLoc;
					return;
				}
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
					if (!results[i].GetActor()->Tags.Contains(Tags::Destroy) || !results[i].GetActor()->Tags.Contains(Tags::Pickup))
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
				FVector previousLoc = nextLoc;

				nextLoc = loc + (rightAxis * moveDistance);
				nextLoc.X = FMath::RoundToFloat(nextLoc.X);
				nextLoc.Y = FMath::RoundToFloat(nextLoc.Y);
				nextLoc.Z = FMath::RoundToFloat(nextLoc.Z);

				if (!GetWorld()->LineTraceSingleByChannel(moveHit, nextLoc, nextLoc - GetActorUpVector() * maxFallDistance, ECC_WorldStatic, moveParams))
				{
					nextLoc = previousLoc;
					return;
				}
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

void AMecha::SetScan()
{
	if (scanWidget && useWidget && (inventoryWidget->IsInViewport() == false))
	{
		if (scanning == true)
		{
			scanWidget->RemoveFromViewport();
			scanning = false;
		}
		else if (scanning == false)
		{
			scanWidget->AddToViewport();
			useWidget->RemoveFromViewport();
			scanning = true;
		}
	}
}

void AMecha::SetWayPoint()
{
	//WAYPOINT
	if (wayPoint)
	{
		FHitResult wayPointHit;
		if (GetWorld()->LineTraceSingleByChannel(wayPointHit, camera->GetComponentLocation(),
			camera->GetComponentLocation() + camera->GetForwardVector() * scanDistance, ECC_WorldStatic))
		{
			wayPoint->SetVisibility(true); //Only here because initial visibility is off
			wayPoint->SetWorldLocation(wayPointHit.ImpactPoint);
			wayPoint->SetWorldRotation(wayPointHit.ImpactNormal.Rotation());
		}
	}
}

void AMecha::RightMousePressed()
{
	//USING
	if (!scanning)
	{
		if (GetWorld()->LineTraceSingleByChannel(useHit, GetActorLocation(), GetActorLocation() + camera->GetForwardVector() * useDistance,
			ECC_WorldStatic))
		{
			AActor* useActor = useHit.GetActor();
			if (useActor->Tags.Contains(Tags::Destroy) == false) //Keep this for interactiable actors that can be destroyed and respawned
			{
				IActivate* useable = Cast<IActivate>(useActor);
				if (useable)
				{
					useable->Use();
				}
			}
		}
	}
}

void AMecha::LeftMousePressed()
{
	if (GetWorld()->LineTraceSingleByChannel(shootHit, camera->GetComponentLocation(),
		GetActorLocation() + camera->GetForwardVector() * shootDistance, ECC_Destructible))
	{
		UDestructibleComponent* dc = Cast<UDestructibleComponent>(shootHit.GetComponent());
		if (dc)
		{
			dc->ApplyDamage(destrutibleDamageAmount, shootHit.ImpactPoint, camera->GetForwardVector(), destructibleDamageStrength);
			dc->GetOwner()->SetLifeSpan(5.f);
			dc->GetOwner()->Tags.Add(Tags::Destroy);
		}
	}
}

void AMecha::OpenInventory()
{
	if (inventoryWidget && controller)
	{
		if (inventoryWidget->IsInViewport() == false)
		{
			FInputModeGameAndUI input = FInputModeGameAndUI();
			input.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
			controller->SetInputMode(input);
			controller->bShowMouseCursor = true;
			inventoryWidget->AddToViewport();
		}
		else if (inventoryWidget->IsInViewport() == true)
		{
			controller->SetInputMode(FInputModeGameOnly());
			controller->bShowMouseCursor = false;
			inventoryWidget->RemoveFromViewport();
		}
	}
}
