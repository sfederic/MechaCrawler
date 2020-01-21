#pragma once

#include "Rebuild.generated.h"

/*
* Actors that can be rebuilt
*/

UINTERFACE(BlueprintType)
class MECHACRAWLER_API URebuild : public UInterface
{
	GENERATED_BODY()
};

class MECHACRAWLER_API IRebuild
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void Rebuild() = 0;
};