#pragma once

#include "Activate.generated.h"

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