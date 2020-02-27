// Fill out your copyright notice in the Description page of Project Settings.

#include "Ship.h"
#include "Components/InputComponent.h"
#include "DialogueComponent.h"
#include "Camera/CameraComponent.h"
#include "DestructibleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "GlobalTags.h"
#include "TagNode.h"
#include "ScanData.h"

AShip::AShip()
{
	PrimaryActorTick.bCanEverTick = true;
	shootParams.AddIgnoredActor(this);
}

void AShip::BeginPlay()
{
	Super::BeginPlay();
	
	controller = Cast<APlayerController>(this->GetController());
	
	controller->bShowMouseCursor = true;

	//Init Widgets
	levelEntryWidget = CreateWidget<UEnterLevelWidget>(GetWorld(), levelEntryWidgetClass);
	check(levelEntryWidget);

	textBoxWidget = CreateWidget<UTextBoxWidget>(GetWorld(), textBoxWidgetClass);
	check(textBoxWidget);

	noteReturnFocusWidget = CreateWidget<UUserWidget>(GetWorld(), noteReturnFocusWidgetClass);
	check(noteReturnFocusWidget);

	scanCursorWidget = CreateWidget<UUserWidget>(GetWorld(), scanCursorWidgetClass);
	check(scanCursorWidget);

	controller->SetMouseCursorWidget(EMouseCursor::Default, scanCursorWidget);

	scanWidget = CreateWidget<UScanWidget>(GetWorld(), scanWidgetClass);
	check(scanWidget);

	//Init camera
	camera = FindComponentByClass<UCameraComponent>();
	check(camera);
}

void AShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//DrawDebugLine(GetWorld(), GetActorLocation(), shootHit.ImpactPoint, FColor::Red);

	Scan();
	ScrollText();

	if (velocity <= velocityMin)
	{
		bMovingForward = false;
	}

	if (reverseVelocity >= reverseVelocityMax)
	{
		bReversing = false;
	}

	velocity = FMath::Clamp(velocity, velocityMin, velocityMax);
	reverseVelocity = FMath::Clamp(reverseVelocity, reverseVelocityMin, reverseVelocityMax);

	if (levelEntryWidget->IsInViewport() == false)
	{
		float tempVel = 0.f;

		if (bMovingForward)
		{
			tempVel = velocity;
		}
		 
		if (bReversing)
		{
			tempVel = reverseVelocity;
		}

		SetActorLocation(GetActorLocation() + (GetActorForwardVector() * tempVel), true);
	}
}

void AShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	InputComponent->BindAxis("Right", this, &AShip::RotateRight);
	InputComponent->BindAxis("Left", this, &AShip::RotateLeft);
	//InputComponent->BindAxis("Forward", this, &AShip::RotateUp);
	//InputComponent->BindAxis("Back", this, &AShip::RotateDown);
	InputComponent->BindAxis("LeftMouseHeld", this, &AShip::Accelerate);
	InputComponent->BindAxis("RightMouseHeld", this, &AShip::Reverse);

	InputComponent->BindAction("LeftMouse", EInputEvent::IE_Pressed, this, &AShip::LeftMousePressed);
	InputComponent->BindAction("RightMouse", EInputEvent::IE_Pressed, this, &AShip::RightMousePressed);
	InputComponent->BindAction("Scan", EInputEvent::IE_Pressed, this, &AShip::SetScan);
	InputComponent->BindAction("Tag", EInputEvent::IE_Pressed, this, &AShip::TagActor);
	InputComponent->BindAction("Note", EInputEvent::IE_Pressed, this, &AShip::AddNote);
}

void AShip::RotateRight(float val)
{
	if (levelEntryWidget->IsInViewport() == false && !bDialogueClick)
	{
		if (val)
		{
			FRotator rot = GetActorRotation();
			rot.Yaw += shipRotateSpeed;
			SetActorRotation(rot);
		}
	}
}

void AShip::RotateLeft(float val)
{
	if (levelEntryWidget->IsInViewport() == false && !bDialogueClick)
	{
		if (val)
		{
			FRotator rot = GetActorRotation();
			rot.Yaw -= shipRotateSpeed;
			SetActorRotation(rot);
		}
	}
}

void AShip::RightMousePressed()
{
	if (bTypingNote)
	{
		//UGameplayStatics::PlaySound2D(GetWorld(), soundNoteLoseFocus);
		noteReturnFocusWidget->RemoveFromViewport();
		bTypingNote = false;
		return;
	}
}

void AShip::LeftMousePressed()
{
	if (bTypingNote)
	{
		//UGameplayStatics::PlaySound2D(GetWorld(), soundNoteLoseFocus);
		noteReturnFocusWidget->RemoveFromViewport();
		bTypingNote = false;
		return;
	}

	if (bDialogueClick)
	{
		if (textBoxWidget->bScrollFinished)
		{
			ProgressText();
			return;
		}
	}
	/*else if (GetWorld()->LineTraceSingleByChannel(shootHit, GetActorLocation(), GetActorLocation() + (GetActorForwardVector() * shootDistance), ECC_WorldStatic, shootParams))
	{
		AActor* shotActor = shootHit.GetActor();
		if (shotActor)
		{
			UDestructibleComponent* dc = shotActor->FindComponentByClass<UDestructibleComponent>(); 
			if (dc)
			{
				dc->ApplyDamage(1000.f, shootHit.ImpactPoint, GetActorForwardVector(), 10000.f);
				shotActor->SetLifeSpan(3.0f);
			}
		}
	}*/
}

void AShip::Accelerate(float val)
{
	if (levelEntryWidget->IsInViewport() == false && !bDialogueClick)
	{
		if (val && bReversing == false)
		{
			bMovingForward = true;
			velocity += shipMoveSpeed * 0.6;
		}
		else
		{
			velocity -= shipMoveSpeed * 0.3f;
		}
	}
}

void AShip::Reverse(float val)
{
	if (levelEntryWidget->IsInViewport() == false && !bDialogueClick)
	{
		if (val && bMovingForward == false)
		{
			bReversing = true;
			reverseVelocity -= reverseSpeed * val;
		}
		else
		{
			reverseVelocity += reverseSpeed;
		}
	}
}

void AShip::GetDialogue(AActor* dialogueActor)
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
				dialogue->mainTextBoxTable->GetAllRows<FTextBox>(context, textBoxWidget->textBoxRows);

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
			}
		}
	}
}

void AShip::ProgressText()
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

			//UGameplayStatics::SetGamePaused(GetWorld(), false);
		}
	}
}

void AShip::ScrollText()
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

void AShip::RotateUp(float val)
{
	if (levelEntryWidget->IsInViewport() == false && !bDialogueClick)
	{
		if (val)
		{
			FRotator rot = GetActorRotation();
			rot.Pitch += shipRotateSpeed * 0.5f;
			rot.Pitch = FMath::Clamp(rot.Pitch, -50.f, 50.f);
			SetActorRotation(rot);
		}
	}
}

void AShip::RotateDown(float val)
{
	if (levelEntryWidget->IsInViewport() == false && !bDialogueClick)
	{
		if (val)
		{
			FRotator rot = GetActorRotation();
			rot.Pitch -= shipRotateSpeed * 0.5f;
			rot.Pitch = FMath::Clamp(rot.Pitch, -50.f, 50.f);
			SetActorRotation(rot);
		}
	}
}

void AShip::SetScan()
{
	bIsScanning = !bIsScanning;

	if (bIsScanning)
	{
		scanWidget->AddToViewport();
	}
	else if (!bIsScanning)
	{
		scanWidget->RemoveFromViewport();
	}
}

void AShip::AddNote()
{
	FVector mouseWorld, mouseDirection;
	controller->DeprojectMousePositionToWorld(mouseWorld, mouseDirection);

	UE_LOG(LogTemp, Warning, TEXT("mouse world: %f %f %f"), mouseWorld.X, mouseWorld.Y, mouseWorld.Z);
	UE_LOG(LogTemp, Warning, TEXT("mouse direction: %f %f %f"), mouseDirection.X, mouseDirection.Y, mouseDirection.Z);

	FHitResult noteHit;
	if (GetWorld()->LineTraceSingleByChannel(noteHit, camera->GetComponentLocation(),
		camera->GetComponentLocation() + mouseDirection * scanDistance, ECC_WorldStatic, shootParams))
	{
		if (noteWidgetClass)
		{
			//Place note
			FTransform transform = FTransform();
			transform.SetLocation(noteHit.ImpactPoint);
			transform.SetRotation(FQuat(noteHit.ImpactNormal.Rotation()));
			ANoteNode* noteNode = GetWorld()->SpawnActor<ANoteNode>(noteWidgetClass, transform);

			bTypingNote = true;

			//UGameplayStatics::PlaySound2D(GetWorld(), soundNote);
			noteReturnFocusWidget->AddToViewport();
		}
	}
	else if (GetWorld()->LineTraceSingleByChannel(noteHit, camera->GetComponentLocation(),
		camera->GetComponentLocation() + mouseDirection * scanDistance, ECC_GameTraceChannel1, shootParams))
	{
		if (noteWidgetClass)
		{
			//Place note for transparent objects
			FTransform transform = FTransform();
			transform.SetLocation(noteHit.ImpactPoint);
			transform.SetRotation(FQuat(noteHit.ImpactNormal.Rotation()));
			ANoteNode* noteNode = GetWorld()->SpawnActor<ANoteNode>(noteWidgetClass, transform);

			bTypingNote = true;

			//UGameplayStatics::PlaySound2D(GetWorld(), soundNote);
			noteReturnFocusWidget->AddToViewport();
		}
	}
}

void AShip::TagActor()
{
	FHitResult noteHit;

	FVector mouseWorld, mouseDirection;
	controller->DeprojectMousePositionToWorld(mouseWorld, mouseDirection);

	UE_LOG(LogTemp, Warning, TEXT("mouse world: %f %f %f"), mouseWorld.X, mouseWorld.Y, mouseWorld.Z);
	UE_LOG(LogTemp, Warning, TEXT("mouse direction: %f %f %f"), mouseDirection.X, mouseDirection.Y, mouseDirection.Z);

	FHitResult tagResult;
	if (GetWorld()->LineTraceSingleByChannel(tagResult, GetActorLocation(), camera->GetComponentLocation() + (mouseDirection * scanDistance), ECC_WorldStatic, shootParams))
	{
		UE_LOG(LogTemp, Warning, TEXT("TaggedActor: %s"), *tagResult.GetActor()->GetName());


		if (tagResult.GetActor()->Tags.Contains(Tags::Tagable))
		{
			AActor* tagActor = tagResult.GetActor();

			if (tagActor->Tags.Contains(Tags::Tagged) == false)
			{
				tagActor->Tags.Add(Tags::Tagged);

				//UGameplayStatics::PlaySound2D(GetWorld(), soundTagged);

				//Set Tag node and widget
				FTransform tagTrans = FTransform();
				tagTrans.SetLocation(tagResult.ImpactPoint);
				ATagNode* tagNode = GetWorld()->SpawnActor<ATagNode>(tagWidgetClass, tagTrans);
				UTagDistanceWidget* tagDistWidget = Cast<UTagDistanceWidget>(tagNode->tagWidget->GetUserWidgetObject());
				tagDistWidget->taggedActor = tagResult.GetActor();
				tagNode->taggedActor = tagResult.GetActor();

				tagNode->Tags.Add(*tagActor->GetName()); //Used when need to delte tag node


				UMeshComponent* tagActorMesh = tagActor->FindComponentByClass<UMeshComponent>();
				if (tagActorMesh)
				{
					tagActorMesh->SetRenderCustomDepth(true);
					tagActorMesh->SetCustomDepthStencilValue(255);
				}
			}
			else if (tagActor->Tags.Contains(Tags::Tagged) == true)
			{
				tagActor->Tags.Remove(Tags::Tagged);

				//Remove tag node(what the absolute fuck, but I guess it's still quick)
				TArray<AActor*> tagNodesToRemove;
				UGameplayStatics::GetAllActorsWithTag(GetWorld(), *tagActor->GetName(), tagNodesToRemove);
				for (AActor* actor : tagNodesToRemove)
				{
					actor->Destroy();
				}

				//UGameplayStatics::PlaySound2D(GetWorld(), soundTaggedOff);

				UMeshComponent* tagActorMesh = tagActor->FindComponentByClass<UMeshComponent>();
				if (tagActorMesh)
				{
					tagActorMesh->SetRenderCustomDepth(false);
					tagActorMesh->SetCustomDepthStencilValue(0);
				}
			}

		}
	}
}


void AShip::Scan()
{
	//SCANNING
	if (bIsScanning && scanWidget)
	{
		FVector mouseWorld, mouseDirection;
		controller->DeprojectMousePositionToWorld(mouseWorld, mouseDirection);

		if (GetWorld()->LineTraceSingleByChannel(scanHit, camera->GetComponentLocation(), camera->GetComponentLocation() + mouseDirection * scanDistance, ECC_GameTraceChannel1)) //TransparentScan
		{
			AActor* actor = scanHit.GetActor();
			if (actor)
			{
				UScanData* scanData = actor->FindComponentByClass<UScanData>();
				if (scanData)
				{
					scanWidget->scanEntry = scanData->scanText.ToString();
					scanWidget->scanNameEntry = scanData->scanName;

					if (previousScanHit.GetActor() == nullptr || previousScanHit.GetActor() != scanHit.GetActor())
					{
						//UGameplayStatics::PlaySound2D(GetWorld(), soundScanOverlap);
					}

					previousScanHit = scanHit;


					if (actor->FindComponentByClass<UDialogueComponent>())
					{
						scanWidget->bHasDialouge = true;
						scanWidget->dialogueName = scanData->dialogueName;
					}
				}
				else
				{
					scanWidget->scanEntry = FString(TEXT("No Scan data."));
					scanWidget->scanNameEntry = FString(TEXT("Scanning..."));
					scanWidget->bHasDialouge = false;
					scanWidget->dialogueName = TEXT("");

					previousScanHit = FHitResult();
				}
			}
		}
		else if (GetWorld()->LineTraceSingleByChannel(scanHit, camera->GetComponentLocation(), camera->GetComponentLocation() + mouseDirection * scanDistance, ECC_WorldStatic))
		{
			AActor* actor = scanHit.GetActor();
			if (actor)
			{
				UScanData* scanData = actor->FindComponentByClass<UScanData>();

				if (scanData)
				{
					scanWidget->scanEntry = scanData->scanText.ToString();
					scanWidget->scanNameEntry = scanData->scanName;

					if (previousScanHit.GetActor() == nullptr || previousScanHit.GetActor() != scanHit.GetActor())
					{
						//UGameplayStatics::PlaySound2D(GetWorld(), soundScanOverlap);
					}

					previousScanHit = scanHit;

					if (actor->FindComponentByClass<UDialogueComponent>())
					{
						scanWidget->bHasDialouge = true;
						scanWidget->dialogueName = scanData->dialogueName;
					}

				}
				else
				{
					scanWidget->scanEntry = FString(TEXT("No Scan data."));
					scanWidget->scanNameEntry = FString(TEXT("Scanning..."));
					scanWidget->bHasDialouge = false;
					scanWidget->dialogueName = TEXT("");

					previousScanHit = FHitResult();
				}
			}
		}
		else
		{
			scanWidget->scanEntry = FString(TEXT("No Scan data."));
			scanWidget->scanNameEntry = FString(TEXT("Scanning..."));
			scanWidget->bHasDialouge = false;
			scanWidget->dialogueName = TEXT("");

			previousScanHit = FHitResult();
		}
	}
}
