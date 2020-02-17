// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ScanData.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MECHACRAWLER_API UScanData : public UActorComponent
{
	GENERATED_BODY()

public:	
	UScanData();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString scanText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString scanName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString dialogueName; //Name of the character you can call in scanwidget

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* dialogueImage; //Image of the character you can call in scanwidget
};
