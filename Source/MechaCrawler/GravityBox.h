// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Activate.h"
#include "GridActor.h"
#include "GravityBox.generated.h"

//Actor that moves in an elevator fashion along any axis. Needs to start flush against something

UCLASS()
class MECHACRAWLER_API AGravityBox : public AGridActor, public IActivate
{
	GENERATED_BODY()
	
public:	
	AGravityBox();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void Use() override;
	virtual void Rebuild() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FCollisionQueryParams cableParams;
	class UBoxComponent* boxCollision;
	UParticleSystemComponent* cable;

	class APawn* player;

	UPROPERTY(EditAnywhere)
	UParticleSystem* particleTemplate;

	UPROPERTY(EditAnywhere)
	FVector gravityDirection;

	FVector originalGravityDirection;

	UPROPERTY(EditAnywhere)
	float fallSpeed;
};
