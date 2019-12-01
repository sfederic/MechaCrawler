#pragma once

#include "ScanData.h"
#include "Scannable.generated.h"

UINTERFACE(BlueprintType)
class MECHACRAWLER_API UScannable : public UInterface
{
	GENERATED_BODY()
};

class MECHACRAWLER_API IScannable
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual UScanData* Scan() = 0;
};