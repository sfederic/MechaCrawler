#pragma once

#include "UObject/NameTypes.h"

namespace Tags
{
	static const FName Destroy = "Destroy"; //Object HAS been destroyed (in APEX sense)
	static const FName Useable = "Useable"; //Object will activate Use widget (Interface still called regardless)
	static const FName Pushable = "Pushable"; 
	static const FName Scannable = "Scannable"; 
	static const FName Pickup = "Pickup";
	static const FName MoveThrough = "MoveThrough"; //Means player can walk through transparent actor
	static const FName Enemy = "Enemy";
	static const FName ScanMarked = "ScanMarked";
	static const FName CantMove = "CantMove"; //For when player can't rotate onto object
}
