#pragma once

#include "Activate.generated.h"

/*
* For interactable and connected actors
*/

UINTERFACE(BlueprintType)
class MECHACRAWLER_API UActivate : public UInterface
{
	GENERATED_BODY()
};

class MECHACRAWLER_API IActivate
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void Use() = 0;
};