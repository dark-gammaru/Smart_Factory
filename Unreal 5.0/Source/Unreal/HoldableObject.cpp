
#include "HoldableObject.h"

UHoldableObject::UHoldableObject()
{
	PrimaryComponentTick.bCanEverTick = true;
}


int32 UHoldableObject::GetWeight() {
	return Weight;
}

FString UHoldableObject::GetName() {
	return Name;
}

int32 UHoldableObject::GetPrice() {
	return Price;
}

