// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CloudMoveComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MECHACRAWLER_API UCloudMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCloudMoveComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	class AActor* cloud;

	UPROPERTY(EditAnywhere)
	FVector moveVector;

	FVector scaleVector;

	UPROPERTY(VisibleAnywhere)
	FVector randomScaleVector;

	UPROPERTY(EditAnywhere)
	float moveSpeed;

	float scale;

	UPROPERTY(EditAnywhere)
	float interpScaleSpeed;
};
