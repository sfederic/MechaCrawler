// Fill out your copyright notice in the Description page of Project Settings.

#include "DestructibleActivate.h"
#include "DestructibleComponent.h"

void ADestructibleActivate::BeginPlay()
{
	dc = FindComponentByClass<UDestructibleComponent>();
	check(dc);
}

void ADestructibleActivate::Use()
{ 
	dc->ApplyDamage(10000.f, this->GetActorLocation(), GetActorForwardVector(), 1000.f);
}
