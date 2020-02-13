// Fill out your copyright notice in the Description page of Project Settings.

#include "Ship.h"
#include "Components/InputComponent.h"
#include "DialogueComponent.h"
#include "Camera/CameraComponent.h"
#include "DestructibleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

AShip::AShip()
{
	PrimaryActorTick.bCanEverTick = true;
	shootParams.AddIgnoredActor(this);
}

void AShip::BeginPlay()
{
	Super::BeginPlay();
	
	controller = Cast<APlayerController>(this->GetController());

	//Init Widgets
	levelEntryWidget = CreateWidget<UEnterLevelWidget>(GetWorld(), levelEntryWidgetClass);
	check(levelEntryWidget);

	textBoxWidget = CreateWidget<UTextBoxWidget>(GetWorld(), textBoxWidgetClass);
	check(textBoxWidget);

	camera = FindComponentByClass<UCameraComponent>();
	check(camera);
}

void AShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugLine(GetWorld(), GetActorLocation(), shootHit.ImpactPoint, FColor::Red);

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
		//TODO: Got rid of reversing. having two SetActorLocations was messing with bSweep?
	}
}

void AShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	InputComponent->BindAxis("Right", this, &AShip::RotateRight);
	InputComponent->BindAxis("Left", this, &AShip::RotateLeft);
	InputComponent->BindAxis("Forward", this, &AShip::RotateUp);
	InputComponent->BindAxis("Back", this, &AShip::RotateDown);
	InputComponent->BindAxis("LeftMouseHeld", this, &AShip::Accelerate);
	InputComponent->BindAxis("RightMouseHeld", this, &AShip::Reverse);

	InputComponent->BindAction("LeftMouse", EInputEvent::IE_Pressed, this, &AShip::LeftMousePressed);
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

void AShip::LeftMousePressed()
{
	if (bDialogueClick)
	{
		if (textBoxWidget->bScrollFinished)
		{
			ProgressText();
			return;
		}
	}
	else if (GetWorld()->LineTraceSingleByChannel(shootHit, GetActorLocation(), GetActorLocation() + (GetActorForwardVector() * shootDistance), ECC_WorldStatic, shootParams))
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
	}
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
