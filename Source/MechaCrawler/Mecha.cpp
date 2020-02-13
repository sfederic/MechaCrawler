// Fill out your copyright notice in the Description page of Project Settings.

#include "Mecha.h"
#include "Kismet/GameplayStatics.h"
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
#include "TimerManager.h" //What was this things problem before?
#include "IceComponent.h"
#include "Enemy.h"
#include "Weapon.h"
#include "DialogueComponent.h"
#include "ProceduralMeshComponent/Public/KismetProceduralMeshLibrary.h"
#include "WeaponData.h"
#include "Engine/PostProcessVolume.h"
#include "Components/StaticMeshComponent.h"
#include "DestructibleMesh.h"
#include "Rebuild.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "DestructibleSwitch.h"
#include "DestructibleActivate.h"
#include "RebuildTransparent.h"
#include "Components/BoxComponent.h"
#include "Pickup.h"

AMecha::AMecha()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true; //Seems to fix fall through on play start when Mech starts on object
	PrimaryActorTick.TickGroup = TG_PrePhysics; //Seems to fix fall through floor effect. Other GridActors needs the same
	
	moveParams.AddIgnoredActor(this);
}

void AMecha::BeginPlay()
{
	Super::BeginPlay();

	initialMoveSpeed = moveSpeed;

	//Spawn Rebuild Manager
	instancedRebuildManager = GetWorld()->SpawnActor<ARebuildManager>();

	//SETUP WEAPONS
	weapons = GetComponentsByClass(UChildActorComponent::StaticClass());
	for (int i = 0; i < weapons.Num(); i++)
	{
		UChildActorComponent* weapon = Cast<UChildActorComponent>(weapons[i]);
		if (weapon)
		{
			weapon->SetVisibility(true);
			UWeaponData* weaponData = weapon->GetChildActor()->FindComponentByClass<UWeaponData>();
			if (weaponData)
			{
				attackDistance = weaponData->range;
			}

			break;
		}
	}

	//INIT WIDGETS
	if (bStartLevelOnShip)
	{
		startLevelWidget = CreateWidget<UUserWidget>(GetWorld(), startLevelWidgetClass);
		startLevelWidget->AddToViewport();
	}

	textBoxWidget = CreateWidget<UTextBoxWidget>(GetWorld(), textBoxWidgetClass);
	if(textBoxWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Dialogue Widget not set in Mecha.cpp"));
	}

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
	lastLoc = currentLoc;

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

	Scan();
	ScrollText();

	//TODO: Remove this and call it through RebuildManager's Tick
	if (instancedRebuildManager)
	{
		instancedRebuildManager->RebuildTimers();
	}

	//MOVEMENT AXIS
	rootAxes[0] = RootComponent->GetForwardVector();
	rootAxes[1] = -RootComponent->GetForwardVector();
	rootAxes[2] = RootComponent->GetRightVector();
	rootAxes[3] = -RootComponent->GetRightVector();

	float forwardAngle = INFINITY;
	float rightAngle = INFINITY;

	forwardAxisIndex = 0;
	rightAxisIndex = 0;

	for (int i = 0; i < 6; i++)
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

	//TODO: Don't know if need water to change move speed
	/*if (submerged)
	{
		moveSpeed = initialMoveSpeed;
	}*/

	if (!GetWorld()->LineTraceSingleByChannel(moveHit, GetActorLocation(), GetActorLocation() - (RootComponent->GetUpVector() * traceDistance), ECC_WorldStatic, moveParams))
	{
		falling = true;
	}

	//GRAVITY
	if (currentLoc == nextLoc && currentRot == nextRot && bStartLevelOnShip == false)
	{
		FVector loc = GetActorLocation();

		//For Water and obstacles that the player can pass through (Tagged as MoveThrough)
		/*if (GetWorld()->LineTraceSingleByChannel(moveHit, loc, loc - (RootComponent->GetUpVector() * traceDistance), ECC_WorldStatic, moveParams))
		{
			if (moveHit.GetActor()->Tags.Contains(Tags::MoveThrough))
			{
				submerged = true;

				nextLoc = loc - (RootComponent->GetUpVector() * moveDistance);
				nextLoc.X = FMath::RoundToFloat(nextLoc.X);
				nextLoc.Y = FMath::RoundToFloat(nextLoc.Y);
				nextLoc.Z = FMath::RoundToFloat(nextLoc.Z);
			}
		}*/

		//For normal gravity
		//if (submerged == false)
		{
			if (!GetWorld()->LineTraceSingleByChannel(moveHit, loc, loc - (RootComponent->GetUpVector() * traceDistance), ECC_WorldStatic, moveParams))
			{
				falling = true;

				//TODO: Acellerating fallspeed mucks with linetrace collision. Might add back in. Might take out. Feels good though.
				moveSpeed += FApp::GetDeltaTime() + 100.0f;

				nextLoc = loc - (RootComponent->GetUpVector() * moveDistance);
				//nextLoc.X = FMath::RoundToFloat(nextLoc.X);
				//nextLoc.Y = FMath::RoundToFloat(nextLoc.Y);
				//nextLoc.Z = FMath::RoundToFloat(nextLoc.Z);
			}
			else
			{
				moveSpeed = previousMoveSpeed;
				falling = false;
			}
		}
	}

	UseObject(); //For 'Use' UI

	//Actor/Camera Rotation & Movement
	camera->SetRelativeRotation(cameraRot);

	currentRot = FMath::QInterpConstantTo(currentRot, nextRot, DeltaTime, rotateSpeed);
	RootComponent->SetRelativeRotation(currentRot);

	currentLoc = FMath::VInterpConstantTo(currentLoc, nextLoc, DeltaTime, moveSpeed);
	SetActorLocation(currentLoc);


	//Rebuild Postprocess effect (fade out)
	if (bFadeOutRebuild)
	{
		fadeOutTimer += FApp::GetDeltaTime();

		const float fadeOutMultiplier = 30.f;

		postProcessMain->Settings.bOverride_VignetteIntensity = 1;
		postProcessMain->Settings.bOverride_DepthOfFieldVignetteSize = 1;

		if (fadeOutTimer < 1.0f)
		{
			postProcessMain->Settings.VignetteIntensity += FApp::GetDeltaTime() * fadeOutMultiplier;
			postProcessMain->Settings.DepthOfFieldVignetteSize += FApp::GetDeltaTime() * fadeOutMultiplier;
		}
		else if (fadeOutTimer > 1.0f)
		{
			postProcessMain->Settings.VignetteIntensity -= FApp::GetDeltaTime() * fadeOutMultiplier;
			postProcessMain->Settings.DepthOfFieldVignetteSize -= FApp::GetDeltaTime() * fadeOutMultiplier;

			if (fadeOutTimer > 2.0f)
			{
				bFadeOutRebuild = false;
				fadeOutTimer = 0.f;
			}
		}
	}
}

void AMecha::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("Forward", this, &AMecha::MoveForward);
	InputComponent->BindAxis("Back", this, &AMecha::MoveBack);
	InputComponent->BindAxis("Left", this, &AMecha::MoveLeft);
	InputComponent->BindAxis("Right", this, &AMecha::MoveRight);
	//InputComponent->BindAxis("Up", this, &AMecha::MoveUp);
	InputComponent->BindAxis("Mouse X", this, &AMecha::LookYaw);
	InputComponent->BindAxis("Mouse Y", this, &AMecha::LookPitch);
	
	//Inputs bound to mouse axis instead of Up/Down. Find out why didn't work.
	InputComponent->BindAxis("MouseWheelUp", this, &AMecha::ZoomIn);
	InputComponent->BindAxis("MouseWheelDown", this, &AMecha::ZoomOut);

	//InputComponent->BindAxis("LeftMouseScan", this, &AMecha::LeftMousePressedScan);
	
	InputComponent->BindAction("LeftMouse", EInputEvent::IE_Pressed, this, &AMecha::LeftMousePressed).bExecuteWhenPaused = true;
	InputComponent->BindAction("Scan", EInputEvent::IE_Pressed, this, &AMecha::SetScan);
	InputComponent->BindAction("RightMouse", EInputEvent::IE_Pressed, this, &AMecha::RightMousePressed).bExecuteWhenPaused = true;
	//InputComponent->BindAction("LeftMouse", EInputEvent::IE_Pressed, this, &AMecha::LeftMousePressed);
	InputComponent->BindAction("Space", EInputEvent::IE_Pressed, this, &AMecha::SetWayPoint);
	InputComponent->BindAction("Enter", EInputEvent::IE_Pressed, this, &AMecha::StartLevel);
	//InputComponent->BindAction("Zoom", EInputEvent::IE_Pressed, this, &AMecha::Zoom);
	InputComponent->BindAction("Note", EInputEvent::IE_Pressed, this, &AMecha::AddNote);
	InputComponent->BindAction("Backspace", EInputEvent::IE_Pressed, this, &AMecha::DeleteAllNotes);
	InputComponent->BindAction("View", EInputEvent::IE_Pressed, this, &AMecha::SetCameraView);
	InputComponent->BindAction("SwitchWeapon", EInputEvent::IE_Pressed, this, &AMecha::ChangeWeapon);
	InputComponent->BindAction("ProgressText", EInputEvent::IE_Pressed, this, &AMecha::ProgressText);
	InputComponent->BindAction("DashForward", EInputEvent::IE_Pressed, this, &AMecha::DashForward);
}

void AMecha::MoveForward(float val)
{
	if ((val != 0.f) && (falling == false) && (canMove) && (!bDialogueClick) && (!bStartLevelOnShip))
	{
		FVector loc = GetActorLocation();

		if (currentLoc == nextLoc && currentRot == nextRot)
		{
			//TODO: move traces look like shit, find a way to ignore channels/set channels (Was I talking about destruction here?)
			TArray<FHitResult> results;
			if (GetWorld()->LineTraceMultiByChannel(results, loc, loc + (forwardAxis * traceDistance), ECC_WorldStatic, moveParams))
			{
				for (int i = 0; i < results.Num(); i++)
				{
					if (results[i].GetActor()->Tags.Contains(Tags::CantMove))
					{
						return;
					}

					//TODO: Maybe get rid of MoveThrough
					if (results[i].GetActor()->Tags.Contains(Tags::MoveThrough))
					{
						nextLoc = loc + (forwardAxis * moveDistance);
						//nextLoc.X = FMath::RoundToFloat(nextLoc.X);
						//nextLoc.Y = FMath::RoundToFloat(nextLoc.Y);
						//nextLoc.Z = FMath::RoundToFloat(nextLoc.Z);

						return;
					}


					if (!results[i].GetActor()->Tags.Contains(Tags::Destroy) && !results[i].GetActor()->Tags.Contains(Tags::Pickup))
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

				//nextRot.Normalize();

				//nextLoc.X = FMath::RoundToFloat(nextLoc.X);
				//nextLoc.Y = FMath::RoundToFloat(nextLoc.Y);
				//nextLoc.Z = FMath::RoundToFloat(nextLoc.Z);
			}
			else
			{
				FVector previousLoc = nextLoc;
				lastLoc = currentLoc;

				nextLoc = loc + (forwardAxis * moveDistance);
				//nextLoc.X = FMath::RoundToFloat(nextLoc.X);
				//nextLoc.Y = FMath::RoundToFloat(nextLoc.Y);
				//nextLoc.Z = FMath::RoundToFloat(nextLoc.Z);

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
	if ((val != 0.f) && (falling == false) && (canMove) && (!bDialogueClick) && (!bStartLevelOnShip))
	{
		FVector loc = GetActorLocation();

		if (currentLoc == nextLoc && currentRot == nextRot)
		{
			TArray<FHitResult> results;
			if (GetWorld()->LineTraceMultiByChannel(results, loc, loc - (forwardAxis * traceDistance), ECC_WorldStatic, moveParams))
			{
				for (int i = 0; i < results.Num(); i++)
				{
					if (results[i].GetActor()->Tags.Contains(Tags::CantMove))
					{
						return;
					}

					if (results[i].GetActor()->Tags.Contains(Tags::MoveThrough))
					{
						nextLoc = loc - (forwardAxis * moveDistance);
						//nextLoc.X = FMath::RoundToFloat(nextLoc.X);
						//nextLoc.Y = FMath::RoundToFloat(nextLoc.Y);
						//nextLoc.Z = FMath::RoundToFloat(nextLoc.Z);

						return;
					}

					if (!results[i].GetActor()->Tags.Contains(Tags::Destroy) && !results[i].GetActor()->Tags.Contains(Tags::Pickup))
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

				//nextRot.Normalize();
			}
			else
			{
				FVector previousLoc = nextLoc;
				lastLoc = currentLoc;

				nextLoc = loc - (forwardAxis * moveDistance);
				//nextLoc.X = FMath::RoundToFloat(nextLoc.X);
				//nextLoc.Y = FMath::RoundToFloat(nextLoc.Y);
				//nextLoc.Z = FMath::RoundToFloat(nextLoc.Z);

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
	if ((val != 0.f) && (falling == false) && (canMove) && (!bDialogueClick) && (!bStartLevelOnShip))
	{
		FVector loc = GetActorLocation();

		if (currentLoc == nextLoc && currentRot == nextRot)
		{
			TArray<FHitResult> results;
			if (GetWorld()->LineTraceMultiByChannel(results, loc, loc - (rightAxis * traceDistance), ECC_WorldStatic, moveParams))
			{
				for (int i = 0; i < results.Num(); i++)
				{
					if (results[i].GetActor()->Tags.Contains(Tags::CantMove))
					{
						return;
					}

					if (results[i].GetActor()->Tags.Contains(Tags::MoveThrough))
					{
						nextLoc = loc - (rightAxis * moveDistance);
						//nextLoc.X = FMath::RoundToFloat(nextLoc.X);
						//nextLoc.Y = FMath::RoundToFloat(nextLoc.Y);
						//nextLoc.Z = FMath::RoundToFloat(nextLoc.Z);

						return;
					}

					if (!results[i].GetActor()->Tags.Contains(Tags::Destroy) && !results[i].GetActor()->Tags.Contains(Tags::Pickup))
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

				//nextRot.Normalize();
			}
			else
			{
				FVector previousLoc = nextLoc;
				lastLoc = currentLoc;

				nextLoc = loc - (rightAxis * moveDistance);
				//nextLoc.X = FMath::RoundToFloat(nextLoc.X);
				//nextLoc.Y = FMath::RoundToFloat(nextLoc.Y);
				//nextLoc.Z = FMath::RoundToFloat(nextLoc.Z);

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
	if ((val != 0.f) && (falling == false) && (canMove) && (!bDialogueClick) && (!bStartLevelOnShip))
	{
		FVector loc = GetActorLocation();

		if (currentLoc == nextLoc && currentRot == nextRot)
		{
			TArray<FHitResult> results;
			if (GetWorld()->LineTraceMultiByChannel(results, loc, loc + (rightAxis * traceDistance), ECC_WorldStatic, moveParams))
			{
				for (int i = 0; i < results.Num(); i++)
				{
					if (results[i].GetActor()->Tags.Contains(Tags::CantMove))
					{
						return;
					}

					if (results[i].GetActor()->Tags.Contains(Tags::MoveThrough))
					{
						nextLoc = loc + (rightAxis * moveDistance);
						//nextLoc.X = FMath::RoundToFloat(nextLoc.X);
						//nextLoc.Y = FMath::RoundToFloat(nextLoc.Y);
						//nextLoc.Z = FMath::RoundToFloat(nextLoc.Z);

						return;
					}

					if (!results[i].GetActor()->Tags.Contains(Tags::Destroy) && !results[i].GetActor()->Tags.Contains(Tags::Pickup))
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

				//nextRot.Normalize();
			}
			else
			{
				FVector previousLoc = nextLoc;
				lastLoc = currentLoc;

				nextLoc = loc + (rightAxis * moveDistance);
				//nextLoc.X = FMath::RoundToFloat(nextLoc.X);
				//nextLoc.Y = FMath::RoundToFloat(nextLoc.Y);
				//nextLoc.Z = FMath::RoundToFloat(nextLoc.Z);

				if (!GetWorld()->LineTraceSingleByChannel(moveHit, nextLoc, nextLoc - GetActorUpVector() * maxFallDistance, ECC_WorldStatic, moveParams))
				{
					nextLoc = previousLoc;
					return;
				}
			}
		}
	}
}

void AMecha::MoveUp(float val)
{
	if (val > 0.0f && submerged)
	{
		nextLoc += FVector::UpVector * moveDistance;
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
	if (bDialogueClick)
	{
		return;
	}

	//TODO: Find a better representation for actors that can be destroyed with scanner
	if (!scanning)
	{
		if (postProcessMain)
		{
			postProcessMain->Settings.AddBlendable(scanPostProcess, 1.0f);
		}

		TArray<AActor*> destroyableScanActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADestructibleActor::StaticClass(), destroyableScanActors);

		for (int i = 0; i < destroyableScanActors.Num(); i++)
		{
			//destroyableScanActors[i]->FindComponentByClass<UDestructibleComponent>()->SetMaterial(0, destroyableWireframeMaterial);
		}

		//Hide Weapon
		weapons[currentWeaponIndex]->GetOwner()->SetActorHiddenInGame(true);
	} 
	else if (scanning)
	{
		if (postProcessMain)
		{
			postProcessMain->Settings.RemoveBlendable(scanPostProcess);
		}

		TArray<AActor*> destroyableScanActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADestructibleActor::StaticClass(), destroyableScanActors);

		for (int i = 0; i < destroyableScanActors.Num(); i++)
		{
			//destroyableScanActors[i]->FindComponentByClass<UDestructibleComponent>()->SetMaterial(0, destroyableBaseMaterial);
		}

		//Hide Weapon
		weapons[currentWeaponIndex]->GetOwner()->SetActorHiddenInGame(false);
	}

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
	if (scanning && bDialogueClick == false)
	{
		if (scanHit.GetActor())
		{
			GetDialogue(scanHit.GetActor());
			return;
		}
	}

	//if (!scanning)
	{
		//Just a debug thing for moving through dialogue faster
		if (bDialogueClick)
		{
			bDialogueClick = false;
			textBoxWidget->textBoxIndex = 0;
			textBoxWidget->textBoxRows.Empty();
			textBoxWidget->RemoveFromViewport();

			//if (textBoxWidget->bScrollFinished)
			{
				//ProgressText();
			}

			return;
		}

		//For Rebuild Transparents
		FHitResult rebuildTransparentHit;
		if (GetWorld()->LineTraceSingleByChannel(rebuildTransparentHit, GetActorLocation(), GetActorLocation() + camera->GetForwardVector() * useDistance, ECC_GameTraceChannel1))
		{
			if (rebuildTransparentHit.GetActor()->IsA<ARebuildTransparent>())
			{
				ARebuildTransparent* rebuildTransparent = Cast<ARebuildTransparent>(rebuildTransparentHit.GetActor());
				if (rebuildTransparent->bRebuilt == false)
				{
					UMeshComponent* rebuildTransparentMesh = rebuildTransparent->FindComponentByClass<UMeshComponent>();
					rebuildTransparentMesh->SetMaterial(0, rebuildTransparent->rebuildMaterial);
					rebuildTransparentMesh->SetCollisionResponseToAllChannels(ECR_Block);
					rebuildTransparent->bRebuilt = true;
					rebuildTransparent->AddOverlap();
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
						rebuildTransparent->particleSpawnTemplate,
						rebuildTransparent->GetActorLocation(), FRotator(), true);
				}
			}
		}

		if (GetWorld()->LineTraceSingleByChannel(useHit, GetActorLocation(), GetActorLocation() + camera->GetForwardVector() * useDistance, ECC_WorldStatic))
		{
			AActor* useActor = useHit.GetActor();
			if (useActor)
			{
				UIceComponent* ice = useActor->FindComponentByClass<UIceComponent>();
				if (ice)
				{
					ice->TurnToWater();
					return;
				}

				//TODO: Cleanup
				/*UDialogueComponent* dialogueComponent = useActor->FindComponentByClass<UDialogueComponent>();
				if (dialogueComponent && textBoxWidget->IsInViewport() == false)
				{
					bDialogueClick = true;

					FString context;
					dialogueComponent->mainTextBoxTable->GetAllRows<FTextBox>(context, textBoxRows);
					textBoxIndex = 0;
					if (textBoxRows.Num() > 0)
					{
						textBoxWidget->name = textBoxRows[textBoxIndex]->name;
						textBoxWidget->text = textBoxRows[textBoxIndex]->text;
						textBoxWidget->image = textBoxRows[textBoxIndex]->image;
						textBoxWidget->AddToViewport();
					}

					return;
				}*/

				if (useActor->Tags.Contains(Tags::Destroy) == false) //Keep this for interactiable actors that can be destroyed and respawned
				{
					IActivate* useable = Cast<IActivate>(useActor);
					if (useable)
					{
						useable->Use();
					}
				}

				if (useActor->Tags.Contains(Tags::Pickup))
				{
					//TODO: Add to some invecntory and a particle effect
					//useActor->Destroy();
					useActor->SetActorHiddenInGame(true);
					useActor->SetActorEnableCollision(false);
					//useActor->SetActorTickEnabled(false);

					if (useActor->IsA<APickup>())
					{
						APickup* pickup = Cast<APickup>(useActor);
						if (pickup)
						{
							pickup->isInInventory = true;
							this->pickups.Add(pickup->pickupType);
						}
					}
				}
			}
		}
	}
}

//SHOOTING
void AMecha::LeftMousePressed()
{
	if (bDialogueClick)
	{
		if (textBoxWidget->bScrollFinished)
		{
			ProgressText();
			return;
		}
	}

	if (scanning)
	{
		/*AActor* scanActor = scanHit.GetActor();
		UDialogueComponent* scanDialogue = scanActor->FindComponentByClass<UDialogueComponent>();
		if (scanDialogue)
		{
			if (textBoxWidget->IsInViewport() == false)
			{
				bDialogueClick = true;

				FString context;
				scanDialogue->mainTextBoxTable->GetAllRows<FTextBox>(context, textBoxRows);
				textBoxIndex = 0;
				if (textBoxRows.Num() > 0)
				{
					textBoxWidget->name = textBoxRows[textBoxIndex]->name;
					textBoxWidget->text = textBoxRows[textBoxIndex]->text;
					textBoxWidget->image = textBoxRows[textBoxIndex]->image;
					textBoxWidget->AddToViewport();
				}

				return;
			}
		}*/

		if (UGameplayStatics::IsGamePaused(GetWorld()) && scanWidget->IsInViewport())
		{
			scanWidget->scanNameEntry = TEXT("Scanning...");
			scanWidget->scanEntry = TEXT("No Entry");

			UGameplayStatics::SetGamePaused(GetWorld(), false);

			return;
		}
		
		if (scanWidget->IsInViewport())
		{
			Scan();
		}
	}

	//if (val)
	{
		//TODO: put cam shake into weapon blueprint
		//UGameplayStatics::PlayWorldCameraShake(GetWorld(), cameraShake, FVector(0.f), 500.f, 1.f); 

		UChildActorComponent* iceWeapon = Cast<UChildActorComponent>(weapons[currentWeaponIndex]);
		if (iceWeapon)
		{
			UWeaponData* weaponData = iceWeapon->GetChildActor()->FindComponentByClass<UWeaponData>();
			if (weaponData)
			{
				if (weaponData->ice)
				{
					if (GetWorld()->LineTraceSingleByChannel(shootHit, camera->GetComponentLocation(),
						GetActorLocation() + camera->GetForwardVector() * attackDistance, ECC_GameTraceChannel1))
					{
						if (shootHit.GetActor()->Tags.Contains(Tags::Water))
						{
							FTransform transform = {};
							transform.SetLocation(shootHit.GetActor()->GetActorLocation());
							transform.SetScale3D(shootHit.GetActor()->GetActorScale3D());
							transform.SetRotation(FQuat(shootHit.GetActor()->GetActorRotation()));

							if (iceBlockClass)
							{
								TArray<AActor*> existingIceBlocks;
								UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ADestructibleActor::StaticClass(), Tags::Ice, existingIceBlocks);
								for (int i = 0; i < existingIceBlocks.Num(); i++)
								{
									if (existingIceBlocks[i]->GetActorLocation().Equals(transform.GetLocation()))
									{
										UE_LOG(LogTemp, Warning, TEXT("Create ice block conflicting with existing location"));
										return;
									}
								}

								instancedRebuildManager->normalRebuildActors.Add(shootHit.GetActor());
								shootHit.GetActor()->SetActorHiddenInGame(true);
								shootHit.GetActor()->SetActorEnableCollision(false);

								ADestructibleActor* iceBlock = GetWorld()->SpawnActor<ADestructibleActor>(iceBlockClass, transform);
								//iceBlock->SetActorScale3D(shootHit.GetActor()->GetActorScale3D);

								iceBlock->Tags.Add(Tags::DontRebuild);
								return;
							}
						}
					}
				}
			}
		}

		if (GetWorld()->LineTraceSingleByChannel(shootHit, camera->GetComponentLocation(),
			GetActorLocation() + camera->GetForwardVector() * attackDistance, ECC_WorldStatic)) 
		{
			UE_LOG(LogTemp, Warning, TEXT("Shot Actor: %s\n"), *shootHit.GetActor()->GetName());

			AActor* shotEnemy = shootHit.GetActor();
			
			if (shotEnemy->Tags.Contains(Tags::Enemy) && shotEnemy->Tags.Contains(Tags::Destroy) == false)
			{
				UDestructibleComponent* enemyDc = shotEnemy->FindComponentByClass<UDestructibleComponent>();
				if (enemyDc)
				{
					enemyDc->ApplyDamage(destructibleDamageAmount, shootHit.ImpactPoint, camera->GetForwardVector(), destructibleDamageStrength);
					shotEnemy->SetLifeSpan(3.0);
					shotEnemy->Tags.Add(Tags::Destroy);
				}
				else
				{
					if (shotEnemy->Tags.Contains(Tags::Destroy) == false)
					{
						AEnemy* enemyCast = Cast<AEnemy>(shotEnemy);
						if (enemyCast)
						{
							enemyCast->DropLoot();
						}

						//shotEnemy->Destroy();
						shotEnemy->FindComponentByClass<UMeshComponent>()->SetSimulatePhysics(true);
						shotEnemy->FindComponentByClass<UMeshComponent>()->AddImpulse(shotEnemy->GetActorForwardVector() * 10000.f);
						shotEnemy->SetActorTickEnabled(false);
						shotEnemy->FindComponentByClass<UBoxComponent>()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
						shotEnemy->Tags.Add(Tags::Destroy);
					}
				}

				return;
			}

			//MESH SLICING
			UProceduralMeshComponent* cutMesh = shootHit.GetActor()->FindComponentByClass<UProceduralMeshComponent>();
			if (cutMesh)
			{
				cutMesh->GetOwner()->Tags.Add(Tags::Destroy);
				cutMesh->SetSimulatePhysics(true);

				UProceduralMeshComponent* newHalfCutMesh;
				UKismetProceduralMeshLibrary::SliceProceduralMesh(cutMesh, shootHit.ImpactPoint, shootHit.ImpactPoint,
					true, newHalfCutMesh, EProcMeshSliceCapOption::CreateNewSectionForCap, destroyableBaseMaterial); //TODO: Change material
				newHalfCutMesh->SetSimulatePhysics(true);
				newHalfCutMesh->AddImpulse(FVector(10000.f));
				newHalfCutMesh->GetOwner()->Tags.Add(Tags::Destroy);

				return;
			}
		}

		if (GetWorld()->LineTraceSingleByChannel(shootHit, camera->GetComponentLocation(),
			GetActorLocation() + camera->GetForwardVector() * attackDistance, ECC_Destructible))
		{
			if (!shootHit.GetActor())
			{
				return; //Works as a fallthrough for the rebuild mechanic.
			}


			//Old tag method
			/*if (shootHit.GetActor()->Tags.Contains(Tags::Destroy))
			{
				UDestructibleComponent* dc = Cast<UDestructibleComponent>(shootHit.GetComponent());

				dc->ApplyDamage(destrutibleDamageAmount, shootHit.ImpactPoint, camera->GetForwardVector(), destructibleDamageStrength);
				return;
			}*/

			UDestructibleComponent* dc = Cast<UDestructibleComponent>(shootHit.GetComponent());

			//For destructible activators
			if (dc)
			{
				if (dc->GetOwner()->IsA<ADestructibleActivate>())
				{
					return;
				}
			}

			ADestructibleActor* rebuildActor = nullptr;
			if (dc)
			{
				rebuildActor = Cast<ADestructibleActor>(dc->GetOwner());
			}
			
			if (dc)
			{
				UChildActorComponent* weapon = Cast<UChildActorComponent>(weapons[currentWeaponIndex]);
				if (weapon)
				{
					UWeaponData* weaponData = weapon->GetChildActor()->FindComponentByClass<UWeaponData>();
					if (weaponData)
					{
						if (weaponData->explosive)
						{
							if (dc->GetOwner()->Tags.Contains(Tags::Destroy) == false)
							{
								UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionParticle, shootHit.ImpactPoint);
							}
						}

						if (weaponData->weaponParticle)
						{
							UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), weaponData->weaponParticle, shootHit.ImpactPoint);
						}
					}
				}

				dc->ApplyDamage(destructibleDamageAmount, shootHit.ImpactPoint, camera->GetForwardVector(), destructibleDamageStrength);	

				if (instancedRebuildManager && dc->GetOwner()->Tags.Contains(Tags::Destroy) == false)
				{
					if (rebuildActor && rebuildActor->IsA<ADestructibleActor>())
					{
						if (rebuildActor->Tags.Contains(Tags::DontRebuild) == false)
						{
							instancedRebuildManager->rebuildActors.Add(rebuildActor);

							instancedRebuildManager->rebuildActorFadeMaterials.Add(rebuildActor->FindComponentByClass<UDestructibleComponent>()->GetMaterial(0));
						}
						else
						{
							rebuildActor->SetLifeSpan(2.0f);
						}
					}

					instancedRebuildManager->rebuildTimers.Add(0.f);

					//FOR DESTRUCTIBLE SWITCHES
					if (dc->GetOwner()->IsA<ADestructibleSwitch>())
					{
						ADestructibleSwitch* destSwitch = Cast<ADestructibleSwitch>(dc->GetOwner());
						if (destSwitch)
						{
							destSwitch->bDestroyed = true;
						}
					}
				}

				dc->GetOwner()->Tags.Add(Tags::Destroy);
			}
		}
	}
}

//Also starts level from crane
void AMecha::StartLevel()
{
	/*if (bStartLevelOnShip == true)
	{
		bStartLevelOnShip = false;
		startLevelWidget->RemoveFromViewport();
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), cameraShake, GetActorLocation(), 500.f, 1.f); 
		camera->FieldOfView = maxFOV;
	}*/

	//TODO: Function was previously open inventory. Decide if need to keep
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

//TODO: Set in binocular UI for later. Don't need for now.
void AMecha::Zoom()
{
	if (camera)
	{
		if (zoomed == false)
		{
			zoomed = true;
			camera->FieldOfView = initialZoomFOV;
		}
		else if (zoomed == true)
		{
			zoomed = false;
			camera->FieldOfView = maxFOV;
		}
	}
}

void AMecha::AddNote()
{
	FHitResult noteHit;
	if (GetWorld()->LineTraceSingleByChannel(noteHit, camera->GetComponentLocation(),
		camera->GetComponentLocation() + camera->GetForwardVector() * scanDistance, ECC_WorldStatic, moveParams))
	{
		if (noteWidgetClass)
		{
			//Place note
			FTransform transform = FTransform();
			transform.SetLocation(noteHit.ImpactPoint);
			transform.SetRotation(FQuat(noteHit.ImpactNormal.Rotation()));
			ANoteNode* noteNode = GetWorld()->SpawnActor<ANoteNode>(noteWidgetClass, transform);

			//highlight actor
			AActor* noteActor = noteHit.GetActor();

			if (noteActor->Tags.Contains(Tags::ScanMarked) == false)
			{
				noteActor->Tags.Add(Tags::ScanMarked);
				//bRenderCustomDepth needs to be true before runtime, not changed during to work
				UStaticMeshComponent* mesh = noteActor->FindComponentByClass<UStaticMeshComponent>();
				if (mesh)
				{
					if (mesh->bRenderCustomDepth == true)
					{
						mesh->SetCustomDepthStencilValue(1);
					}
				}
			}
		}
	}
}

void AMecha::DeleteAllNotes()
{
	if (GetActorLocation().Equals(nextLoc))
	{
		//Delete all notes
		TArray<AActor*> noteActorsToDelete;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANoteNode::StaticClass(), noteActorsToDelete);
		for (int i = 0; i < noteActorsToDelete.Num(); i++)
		{
			noteActorsToDelete[i]->Destroy(); //TODO: Find a way to cap?
		}

		//Unmark all actors
		TArray<AActor*> markedActorsToUnmark;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), Tags::ScanMarked, markedActorsToUnmark);
		for (int i = 0; i < markedActorsToUnmark.Num(); i++)
		{
			markedActorsToUnmark[i]->Tags.Remove(Tags::ScanMarked);

			//Couldn't get working for destructible meshes
			UStaticMeshComponent* mesh = markedActorsToUnmark[i]->FindComponentByClass<UStaticMeshComponent>();
			if (mesh)
			{
				if (mesh->bRenderCustomDepth == true)
				{
					mesh->SetCustomDepthStencilValue(0);
				}
			}
		}

		RebuildAllDestroyedActors();
	}
}

void AMecha::ZoomIn(float val)
{
	if (camera)
	{
		camera->FieldOfView += 5.0f * val; //TODO: Figure out how feels with different mice (TODO: Google plural of mouses)
		camera->FieldOfView = FMath::Clamp(camera->FieldOfView, 5.0f, maxFOV);
	}
}

void AMecha::ZoomOut(float val)
{
	if (camera)
	{
		camera->FieldOfView -= 5.0f * val;
		camera->FieldOfView = FMath::Clamp(camera->FieldOfView, 5.0f, maxFOV);
	}
}

void AMecha::SetCameraView()
{
	if (camera)
	{
		if (camera->ProjectionMode == ECameraProjectionMode::Perspective)
		{
			camera->ProjectionMode = ECameraProjectionMode::Orthographic;
		}
		else if (camera->ProjectionMode == ECameraProjectionMode::Orthographic)
		{
			camera->ProjectionMode = ECameraProjectionMode::Perspective;
		}
	}
}

void AMecha::RebuildAllDestroyedActors()
{
	//bFadeOutRebuild = true;

	TArray<AActor*> dontRebuildActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), Tags::DontRebuild, dontRebuildActors);
	for (int i = 0; i < dontRebuildActors.Num(); i++)
	{
		dontRebuildActors[i]->Destroy();
	}

	if (instancedRebuildManager)
	{
		UWorld* world = GetWorld();


		for (int i = 0; i < instancedRebuildManager->rebuildActors.Num(); i++)
		{
			if (instancedRebuildManager->rebuildActors[i]->GetActorLocation().Equals(GetActorLocation()))
			{
				UE_LOG(LogTemp, Warning, TEXT("DESTRUCTIBLE REBUILD is clashing with player position"));
				return;
			}
		}

		for (int i = 0; i < instancedRebuildManager->normalRebuildActors.Num(); i++)
		{
			if (instancedRebuildManager->normalRebuildActors[i]->GetActorLocation().Equals(GetActorLocation()))
			{
				UE_LOG(LogTemp, Warning, TEXT("NORMAL REBUILD is clashing with player position"));
				return;
			}
		}

		for (int i = 0; i < instancedRebuildManager->rebuildActors.Num(); i++)
		{
			//conditional statement order is important here. Other classes that inherit from ADestructible actor will fall through on that IsA<>()
			if (instancedRebuildManager->rebuildActors[i]->IsA<ADestructibleSwitch>())
			{
				ADestructibleSwitch* da = world->SpawnActor<ADestructibleSwitch>(ADestructibleSwitch::StaticClass(),
					instancedRebuildManager->rebuildActors[i]->GetActorTransform());
				da->FindComponentByClass<UDestructibleComponent>()->SetDestructibleMesh(instancedRebuildManager->rebuildActors[i]->FindComponentByClass<UDestructibleComponent>()->GetDestructibleMesh());
				da->FindComponentByClass<UDestructibleComponent>()->SetMaterial(0, instancedRebuildManager->rebuildActorFadeMaterials[i]);
				da->FindComponentByClass<UDestructibleComponent>()->SetScalarParameterValueOnMaterials("FadeValue", 1.f);
			}
			else if (instancedRebuildManager->rebuildActors[i]->IsA<ADestructibleActor>())
			{
				ADestructibleActor* da = world->SpawnActor<ADestructibleActor>(ADestructibleActor::StaticClass(),
					instancedRebuildManager->rebuildActors[i]->GetActorTransform());
				da->FindComponentByClass<UDestructibleComponent>()->SetDestructibleMesh(instancedRebuildManager->rebuildActors[i]->FindComponentByClass<UDestructibleComponent>()->GetDestructibleMesh());
				da->FindComponentByClass<UDestructibleComponent>()->SetMaterial(0, instancedRebuildManager->rebuildActorFadeMaterials[i]);
				da->FindComponentByClass<UDestructibleComponent>()->SetScalarParameterValueOnMaterials("FadeValue", 1.f);
			}

			instancedRebuildManager->rebuildActors[i]->Destroy();
		}

		for (int i = 0; i < instancedRebuildManager->rebuildActorsActivate.Num(); i++)
		{
			if (instancedRebuildManager->rebuildActorsActivate[i]->IsA<ADestructibleActivate>())
			{
				ADestructibleActivate* da = world->SpawnActor<ADestructibleActivate>(ADestructibleActivate::StaticClass(),
					instancedRebuildManager->rebuildActorsActivate[i]->GetActorTransform());
				//da->switches = instancedRebuildManager->rebuildActorsActivate[i]->switches; //Copy over EditAnywhere switches
				//da->rebuildManager = instancedRebuildManager->rebuildActorsActivate[i]->rebuildManager;
				//da->dc = instancedRebuildManager->rebuildActorsActivate[i]findcom
				da->FindComponentByClass<UDestructibleComponent>()->SetDestructibleMesh(instancedRebuildManager->rebuildActorsActivate[i]->FindComponentByClass<UDestructibleComponent>()->GetDestructibleMesh());
				da->FindComponentByClass<UDestructibleComponent>()->SetMaterial(0, instancedRebuildManager->rebuildActorActivateFadeMaterials[i]);
				da->FindComponentByClass<UDestructibleComponent>()->SetScalarParameterValueOnMaterials("FadeValue", 1.f);

				instancedRebuildManager->rebuildActorsActivate[i]->Destroy();
			}
		}

		for (int i = 0; i < instancedRebuildManager->normalRebuildActors.Num(); i++)
		{
			instancedRebuildManager->normalRebuildActors[i]->SetActorEnableCollision(true);
			instancedRebuildManager->normalRebuildActors[i]->SetActorHiddenInGame(false);
		}

		TArray<AActor*> rebuildActors;
		UGameplayStatics::GetAllActorsWithInterface(GetWorld(), URebuild::StaticClass(), rebuildActors);
		for (int i = 0; i < rebuildActors.Num(); i++)
		{
			IRebuild* rebuilt = Cast<IRebuild>(rebuildActors[i]);
			if (rebuilt)
			{
				rebuilt->Rebuild();
			}
		}

		instancedRebuildManager->rebuildActors.Empty();
		//instancedRebuildManager->normalRebuildActors.Empty();
		instancedRebuildManager->rebuildActorsActivate.Empty();
		instancedRebuildManager->rebuildActorFadeMaterials.Empty();
		instancedRebuildManager->rebuildTimers.Empty();
		instancedRebuildManager->rebuildActivateTimers.Empty();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Rebuild Manager not set in level"));
	}
}

void AMecha::UseObject()
{
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
			if (!lookHit.GetActor())
			{
				return;
			}
			
			if (lookHit.GetActor()->Tags.Contains(Tags::Useable))
			{
				if (useWidget->IsInViewport() == false)
				{
					if (lookHit.GetActor()->Tags.Contains(Tags::Pushable))
					{
						useWidget->useText = "Right Mouse: Push";
					}
					else if (lookHit.GetActor()->Tags.Contains(Tags::Pickup))
					{
						useWidget->useText = "Right Mouse: Pickup";
					}
					else if (lookHit.GetActor()->Tags.Contains(Tags::Talk))
					{
						useWidget->useText = "Right Mouse: Talk";
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
}

void AMecha::ChangeWeapon()
{
	UChildActorComponent* weapon = Cast<UChildActorComponent>(weapons[currentWeaponIndex]);
	if (weapon)
	{
		weapon->SetVisibility(false);
	}

	currentWeaponIndex++;

	if (currentWeaponIndex >= weapons.Num())
	{
		currentWeaponIndex = 0;
	}

	weapon = Cast<UChildActorComponent>(weapons[currentWeaponIndex]);
	if (weapon)
	{
		weapon->SetVisibility(true);
		UWeaponData* weaponData = weapon->GetChildActor()->FindComponentByClass<UWeaponData>();
		if (weaponData)
		{
			attackDistance = weaponData->range;
		}
	}
}

void AMecha::ProgressText()
{
	if (textBoxWidget->IsInViewport())
	{
		if (textBoxWidget->textBoxIndex < (textBoxWidget->textBoxRows.Num() - 1))
		{
			textBoxWidget->textBoxIndex++;
			textBoxWidget->bScrollFinished = false;

			textBoxWidget->name = textBoxWidget->textBoxRows[textBoxWidget->textBoxIndex]->name;
			textBoxWidget->scrollIndex = 0;
			textBoxWidget->text.Empty();
			textBoxWidget->text.AppendChar(textBoxWidget->textBoxRows[textBoxWidget->textBoxIndex]->text[textBoxWidget->scrollIndex]);
			textBoxWidget->image = textBoxWidget->textBoxRows[textBoxWidget->textBoxIndex]->image;
		}
		else
		{
			bDialogueClick = false;
			textBoxWidget->textBoxIndex = 0;
			textBoxWidget->textBoxRows.Empty();
			textBoxWidget->RemoveFromViewport();

			UGameplayStatics::SetGamePaused(GetWorld(), false);
		}
	}
}

void AMecha::ScrollText()
{
	if (textBoxWidget->IsInViewport())
	{
		if (textBoxWidget->scrollIndex < (textBoxWidget->textBoxRows[textBoxWidget->textBoxIndex]->text.Len() - 1))
		{
			textBoxWidget->scrollTimer += FApp::GetDeltaTime() * textBoxWidget->scrollSpeed;
			if (textBoxWidget->scrollTimer > 0.1f)
			{
				textBoxWidget->scrollTimer = 0.f;
				textBoxWidget->scrollIndex++;
				textBoxWidget->text.AppendChar(textBoxWidget->textBoxRows[textBoxWidget->textBoxIndex]->text[textBoxWidget->scrollIndex]);
			}
		}
		else
		{
			textBoxWidget->bScrollFinished = true;
			//textBoxWidget->scrollIndex = 0;
			textBoxWidget->scrollTimer = 0.f;
		}
	}
}

void AMecha::DashForward()
{
	FHitResult dashHit;
	const float dashDistance = 1000.f;
	if (GetWorld()->LineTraceSingleByChannel(dashHit, GetActorLocation(), GetActorLocation() + forwardAxis * dashDistance, ECC_WorldStatic, moveParams))
	{
		nextLoc = dashHit.ImpactPoint - (forwardAxis * 50.f);
		//nextLoc.X = FMath::RoundToFloat(nextLoc.X);
		//nextLoc.Y = FMath::RoundToFloat(nextLoc.Y);
		//nextLoc.Z = FMath::RoundToFloat(nextLoc.Z);

		moveSpeed = dashSpeed;
	}
}

void AMecha::LeftMousePressedScan(float val)
{
	if (val)
	{
		if (UGameplayStatics::IsGamePaused(GetWorld()) && scanWidget->IsInViewport())
		{
			scanWidget->scanNameEntry = TEXT("Scanning...");
			scanWidget->scanEntry = TEXT("No Entry");

			//UGameplayStatics::SetGamePaused(GetWorld(), false);

			return;
		}
	}

	if (scanning && val)
	{
		if (scanWidget->IsInViewport())
		{
			Scan();
		}
	}
}

void AMecha::Scan()
{
	//SCANNING
	if (scanning && scanWidget)
	{
		if (GetWorld()->LineTraceSingleByChannel(scanHit, camera->GetComponentLocation(), camera->GetComponentLocation() + camera->GetForwardVector() * scanDistance, ECC_GameTraceChannel1)) //TransparentScan
		{
			AActor* actor = scanHit.GetActor();
			if (actor)
			{
				UScanData* scanData = actor->FindComponentByClass<UScanData>();
				if (scanData)
				{
					scanWidget->scanEntry = scanData->scanText;
					scanWidget->scanNameEntry = scanData->scanName;

					//UGameplayStatics::SetGamePaused(GetWorld(), true);
				}
				else
				{
					scanWidget->scanEntry = FString(TEXT("No Scan data."));
					scanWidget->scanNameEntry = FString(TEXT("Scanning..."));
				}
			}
		}
		else if (GetWorld()->LineTraceSingleByChannel(scanHit, camera->GetComponentLocation(), camera->GetComponentLocation() + camera->GetForwardVector() * scanDistance, ECC_WorldStatic)) 
		{
			AActor* actor = scanHit.GetActor();
			if (actor)
			{
				UScanData* scanData = actor->FindComponentByClass<UScanData>();
				if (scanData)
				{
					scanWidget->scanEntry = scanData->scanText;
					scanWidget->scanNameEntry = scanData->scanName;

					//UGameplayStatics::SetGamePaused(GetWorld(), true);
				}
				else
				{
					scanWidget->scanEntry = FString(TEXT("No Scan data."));
					scanWidget->scanNameEntry = FString(TEXT("Scanning..."));
				}
			}
		}
		else
		{
			scanWidget->scanEntry = FString(TEXT("No Scan data."));
			scanWidget->scanNameEntry = FString(TEXT("Scanning..."));

			//UGameplayStatics::SetGamePaused(GetWorld(), true);
		}
	}
}

void AMecha::GetDialogue(AActor* dialogueActor)
{
	UDialogueComponent* dialogue = dialogueActor->FindComponentByClass<UDialogueComponent>();
	if (dialogue)
	{
		//UGameplayStatics::SetGamePaused(GetWorld(), true);

		if (textBoxWidget->IsInViewport() == false)
		{
			bDialogueClick = true;

			if (dialogue->mainTextBoxTable)
			{
				FString context;
				if (dialogue->bFirstTextBoxRead == false)
				{
					dialogue->mainTextBoxTable->GetAllRows<FTextBox>(context, textBoxWidget->textBoxRows);
				}
				else if (dialogue->bFirstTextBoxRead == true)
				{
					dialogue->secondTextBoxTable->GetAllRows<FTextBox>(context, textBoxWidget->textBoxRows);
				}

				textBoxWidget->textBoxIndex = 0;

				if (textBoxWidget->textBoxRows.Num() > 0)
				{
					textBoxWidget->name = textBoxWidget->textBoxRows[textBoxWidget->textBoxIndex]->name;
					textBoxWidget->scrollIndex = 0;
					textBoxWidget->text.Empty();
					textBoxWidget->text.AppendChar(textBoxWidget->textBoxRows[textBoxWidget->textBoxIndex]->text[textBoxWidget->scrollIndex]); //Just get the first char, scroll it in ProgressText()
					textBoxWidget->image = textBoxWidget->textBoxRows[textBoxWidget->textBoxIndex]->image;
					textBoxWidget->AddToViewport();
				}

				if (dialogue->secondTextBoxTable)
				{
					dialogue->bFirstTextBoxRead = true;
				}
			}
		}
	}
}
