#pragma once

#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "Activate.h"
#include "TextBox.generated.h"

//Dialogue UI for codec calls. Keep in mind that Uproperty() is needed 

UENUM()
enum class ECharacterNames : uint8
{
	None UMETA(DisplayName="None"),
	Aden UMETA(DisplayName="Aden"),
	Enyel UMETA(DisplayName="Enyel"),
	Gavol UMETA(DisplayName="Gavol"),
	NO_COMM_ID UMETA(DisplayName="NO_COMM_ID"),
};

USTRUCT(BlueprintType)
struct FTextBox : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString text;

	UPROPERTY(EditAnywhere)
	ECharacterNames name;

	UPROPERTY(EditAnywhere)
	UTexture2D* image;

	UPROPERTY(EditAnywhere)
	bool isChoice = false;

	UPROPERTY(EditAnywhere)
	FString choice1;

	UPROPERTY(EditAnywhere)
	bool choice1Chosen = false;

	UPROPERTY(EditAnywhere)
	FString choice2;

	UPROPERTY(EditAnywhere)
	bool choice2Chosen = false;
};