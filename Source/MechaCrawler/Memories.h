#pragma once

//https://wiki.unrealengine.com/EnumToString_Macro
#define GETENUMSTRING(etype, evalue) ( (FindObject<UEnum>(ANY_PACKAGE, TEXT(etype), true) != nullptr) ? FindObject<UEnum>(ANY_PACKAGE, TEXT(etype), true)->GetEnumName((int32)evalue) : FString("Invalid - are you sure enum uses UENUM() macro?") )

UENUM()
enum class EMemories : uint8
{
	TagLevelMemory UMETA(DisplayName="TagLevelMemory")
};