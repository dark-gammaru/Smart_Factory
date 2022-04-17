// Fill out your copyright notice in the Description page of Project Settings.

#include "LineTraceFloorStrategy.h"


LineTraceFloorStrategy::LineTraceFloorStrategy(AActor* OwnerRef, UWorld* WorldRef) : LineTraceStrategy(OwnerRef, WorldRef)
{
	// Floor traces only with wall and floor
	FloorTraceObjectParams.AddObjectTypesToQuery(FLOOR_COLLISION_CHANNEL);
	FloorTraceObjectParams.AddObjectTypesToQuery(WALL_COLLISION_CHANNEL);
}

/// <summary>
/// Only raycast to floor and wall, with FVector from GetPlayersWorldPos() and GetPlayersReach() [LSH]
/// </summary>
/// <returns>Raycast result FHitResult</returns>
FHitResult LineTraceFloorStrategy::GetReach() const {
	FHitResult Hit;
	FCollisionQueryParams FloorQueryParams(FName(TEXT("")), false, OwnerRef);
	WorldRef->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayerLocation(),
		GetPlayersReach(ConstructDistance),
		FloorTraceObjectParams,
		FloorQueryParams
	);
	return Hit;
}
