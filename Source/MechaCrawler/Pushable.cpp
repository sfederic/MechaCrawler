// Fill out your copyright notice in the Description page of Project Settings.


#include "Pushable.h"

// Sets default values
APushable::APushable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APushable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APushable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

