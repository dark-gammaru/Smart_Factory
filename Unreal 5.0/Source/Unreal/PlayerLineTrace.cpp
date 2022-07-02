

#include "PlayerLineTrace.h"

UPlayerLineTrace::UPlayerLineTrace()
{
	PrimaryComponentTick.bCanEverTick = true;
	LineTraceObject = new LineTraceObjectStrategy(GetOwner(), GetWorld());
	LineTraceFloor = new LineTraceFloorStrategy(GetOwner(), GetWorld());
	ActiveLineTrace = LineTraceObject;
}

void UPlayerLineTrace::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerLineTrace::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsConstructing) {
		ActiveLineTrace = LineTraceFloor;
	}
	else {
		ActiveLineTrace = LineTraceObject;
	}
	HitResult = ActiveLineTrace->GetReach();
}

FHitResult UPlayerLineTrace::GetHitResult() {
	return HitResult;
}

FRotator UPlayerLineTrace::GetPlayerRotation() {
	return ActiveLineTrace->GetPlayerRotation();
}
