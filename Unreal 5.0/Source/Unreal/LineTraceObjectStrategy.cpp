// Fill out your copyright notice in the Description page of Project Settings.


#include "LineTraceObjectStrategy.h"
#include "HoldableObject.h"

LineTraceObjectStrategy::LineTraceObjectStrategy(AActor* OwnerRef, UWorld* WorldRef) : LineTraceStrategy(OwnerRef, WorldRef)
{
}

/// <summary>
/// Raycast with FVector from GetPlayersWorldPos() and GetPlayersReach() [LSH]
/// </summary>
/// <returns>Raycast result FHitResult</returns>
FHitResult LineTraceObjectStrategy::GetReach() const {
	FHitResult Hit;
	FVector PlayerLocation = GetPlayerLocation();
	FVector PlayerReach = GetPlayersReach(ObjectDistance);
	FCollisionQueryParams ObjectQueryParams(FName(TEXT("")), false, OwnerRef);
	WorldRef->LineTraceSingleByChannel(
		OUT Hit,
		PlayerLocation,
		PlayerReach,
		ECollisionChannel(ObjectTraceChannel),
		ObjectQueryParams
	);

	FHitResult SecondHit;
	if (auto Comp = Hit.GetComponent()) {
		if (Comp->ComponentHasTag("Incubator")) {
			FVector SecondStart = FMath::Lerp(Hit.Location, PlayerReach, 0.1f);
			WorldRef->LineTraceSingleByChannel(
				OUT SecondHit,
				SecondStart,
				PlayerReach,
				ECollisionChannel(ObjectTraceChannel),
				ObjectQueryParams
			);
			if (SecondHit.GetActor() && SecondHit.GetActor()->FindComponentByClass<UHoldableObject>()) {
				return SecondHit;
			}
		}
	}
	return Hit;
}
