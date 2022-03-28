
#include "HoldableObject.h"

UHoldableObject::UHoldableObject()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

int32 UHoldableObject::GetWeight() {
	return Weight;
}

