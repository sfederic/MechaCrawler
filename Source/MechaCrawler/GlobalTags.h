#pragma once

#include "UObject/NameTypes.h"

namespace Tags
{
	static const FName Destroy = "Destroy";
	static const FName Useable = "Useable"; 
	static const FName Pushable = "Pushable"; 
	static const FName Scannable = "Scannable"; 
	static const FName Pickup = "Pickup";
	static const FName MoveThrough = "MoveThrough"; //Means player can walk through transparent actor
	static const FName Enemy = "Enemy";
	static const FName ScanMarked = "ScanMarked";
}
