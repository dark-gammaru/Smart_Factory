

#include "PlayerLineTrace.h"

UPlayerLineTrace::UPlayerLineTrace()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerLineTrace::BeginPlay()
{
	Super::BeginPlay();

	LineTraceObject = new LineTraceObjectStrategy(GetOwner(), GetWorld());
	LineTraceFloor = new LineTraceFloorStrategy(GetOwner(), GetWorld());
	ActiveLineTrace = LineTraceObject;
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

FHitResult UPlayerLineTrace::ForceLineTraceObject() {
	return LineTraceObject->GetReach();
}
