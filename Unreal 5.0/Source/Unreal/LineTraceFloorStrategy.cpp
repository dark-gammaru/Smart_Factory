// Fill out your copyright notice in the Description page of Project Settings.

#include "LineTraceFloorStrategy.h"


LineTraceFloorStrategy::LineTraceFloorStrategy(AActor* OwnerRef, UWorld* WorldRef) : LineTraceStrategy(OwnerRef, WorldRef)
{
}

/// <summary>
/// Only raycast to floor and wall, with FVector from GetPlayersWorldPos() and GetPlayersReach() [LSH]
/// </summary>
/// <returns>Raycast result FHitResult</returns>
FHitResult LineTraceFloorStrategy::GetReach() const {
	FHitResult Hit;
	FCollisionQueryParams FloorQueryParams(FName(TEXT("")), false, OwnerRef);
	WorldRef->LineTraceSingleByChannel(
		OUT Hit,
		GetPlayerLocation(),
		GetPlayersReach(ConstructDistance),
		ECollisionChannel(FurnatureTraceChannel),
		FloorQueryParams
	);

	// If target is garbage chute, set distance as object distance.
	if (auto HitActor = Hit.GetActor()) {
		if (HitActor->ActorHasTag("GarbageChute")) {
			if (Hit.Distance > ObjectDistance) {
				Hit = FHitResult();
			}
		}
	}
	return Hit;
}
