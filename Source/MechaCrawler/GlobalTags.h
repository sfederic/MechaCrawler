#pragma once

#include "UObject/NameTypes.h"

namespace Tags
{
	static const FName Destroy = "Destroy"; //Object HAS been destroyed (in APEX sense)
	static const FName CantDestroy = "CantDestroy"; //Object CAN'T been destroyed in game
	static const FName Useable = "Useable"; //Object will activate Use widget (Interface still called regardless)
	static const FName Pushable = "Pushable"; 
	static const FName Scan = "Scan"; 
	static const FName Talk = "Talk"; //For NPCs
	static const FName Pickup = "Pickup";
	static const FName MoveThrough = "MoveThrough"; //Means player can walk through actor
	static const FName Enemy = "Enemy";
	static const FName ScanMarked = "ScanMarked";
	static const FName CantMove = "CantMove"; //For when player can't rotate onto object
	static const FName Water = "Water";
	static const FName Ice = "Ice";
	static const FName DontRebuild = "DontRebuild"; //Flaging actors that don't need to be rebuilt (ice block spawns etc.)
	static const FName MechaFirstTime = "MechaFirstTime"; //When mecha first found.
	static const FName Tagged = "Tagged"; //Actors that have had their depth/stencil masks set
	static const FName Tagable = "Tagable"; //Actor that CAN be tagged
	static const FName UseableShoot = "UseableShoot"; //For switches that can be shot to activate
	static const FName ScanUse = "ScanUse"; //Use() for scanner from a distance (like Metroid Prime)
}

namespace LevelNames
{
	static const FString OpeningLevel = TEXT("OpeningLevel");
}

namespace SaveSlots
{
	static const FString Slot1 = TEXT("Slot1");
	static const int slotIndex = 0;
}
