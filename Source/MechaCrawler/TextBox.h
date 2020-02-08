#pragma once

#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "TextBox.generated.h"

//Dialogue UI for codec calls. Keep in mind that Uproperty() is needed 

USTRUCT(BlueprintType)
struct FTextBox : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString text;

	UPROPERTY(EditAnywhere)
	FString name;

	UPROPERTY(EditAnywhere)
	UTexture2D* image;
};