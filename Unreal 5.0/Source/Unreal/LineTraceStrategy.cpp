// Fill out your copyright notice in the Description page of Project Settings.


#include "LineTraceStrategy.h"


LineTraceStrategy::LineTraceStrategy(AActor* OwnerRef, UWorld* WorldRef) : OwnerRef(OwnerRef), WorldRef(WorldRef)
{
}

/// <summary>
/// Get player camera position and return it. [LSH]
/// </summary>
/// <returns>FVector of player camera position</returns>
FVector LineTraceStrategy::GetPlayerLocation() const {
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	// Get player's camera position and rotation
	WorldRef->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	// Return player camera's location
	return PlayerViewPointLocation;
}

/// <summary>
/// Get player camera rotation and return it. [LSH]
/// </summary>
/// <returns>FRotator of player camera rotation.</returns>
FRotator LineTraceStrategy::GetPlayerRotation() const {
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	// Get player's camera position and rotation
	WorldRef->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	// Return player camera's rotation
	return PlayerViewPointRotation;
}


/// <summary>
/// Calculate raycast end point vector with player viewpoint and float HandDistance [LSH]
/// </summary>
/// <returns>FVector of player camera position to raycast end point</returns>
FVector LineTraceStrategy::GetPlayersReach(float Distance) const {
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	// Get player camera's position and rotation
	WorldRef->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	// Make vector from Rotation, Location, Distance
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Distance;
}