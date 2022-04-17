// Fill out your copyright notice in the Description page of Project Settings.


#include "LineTraceObjectStrategy.h"

LineTraceObjectStrategy::LineTraceObjectStrategy(AActor* OwnerRef, UWorld* WorldRef) : LineTraceStrategy(OwnerRef, WorldRef)
{
}

/// <summary>
/// Raycast with FVector from GetPlayersWorldPos() and GetPlayersReach() [LSH]
/// </summary>
/// <returns>Raycast result FHitResult</returns>
FHitResult LineTraceObjectStrategy::GetReach() const {
	FHitResult Hit;
	FCollisionQueryParams ObjectQueryParams(FName(TEXT("")), false, OwnerRef);
	WorldRef->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayerLocation(),
		GetPlayersReach(ObjectDistance),
		FCollisionObjectQueryParams(FCollisionObjectQueryParams::DefaultObjectQueryParam),
		ObjectQueryParams
	);
	return Hit;
}
