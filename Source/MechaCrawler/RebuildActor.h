// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DestructibleActor.h"
#include "RebuildActor.generated.h"

/**
 * Instanced actor for keeping hold of destroyed actor pointers.
 */
UCLASS()
class MECHACRAWLER_API ARebuildActor : public ADestructibleActor
{
	GENERATED_BODY()
};
