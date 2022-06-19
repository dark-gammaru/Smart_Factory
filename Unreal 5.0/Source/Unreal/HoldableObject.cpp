
#include "HoldableObject.h"

int32 UHoldableObject::GetWeight() {
	return Weight;
}

FString UHoldableObject::GetName() {
	return Name;
}

int32 UHoldableObject::GetPrice() {
	return Price;
}

bool UHoldableObject::IsHoldable() {
	return bIsHoldable;
}

void UHoldableObject::MakeUnholdable() {
	bIsHoldable = false;
}

