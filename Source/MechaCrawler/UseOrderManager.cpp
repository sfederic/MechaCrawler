// Fill out your copyright notice in the Description page of Project Settings.


#include "UseOrderManager.h"
#include "ConstructBeacon.h"

AUseOrderManager::AUseOrderManager()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AUseOrderManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void AUseOrderManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (activatedItems.Num() == itemActivateOrder.Num())
	{
		for (int i = 0; i < activatedItems.Num(); i++)
		{
			if (activatedItems[i] != itemActivateOrder[i])
			{
				activatedItems.Empty();
				return;
			}
		}

		activatedItems.Empty();

		IActivate* activate = Cast<IActivate>(useActor);
		if (activate)
		{
				activate->Use();
		}
	}
}

void AUseOrderManager::Rebuild()
{
	activatedItems.Empty();
}
