// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LineTraceStrategy.h"


class UNREAL_API LineTraceFloorStrategy : public LineTraceStrategy
{
	
public:
	FHitResult GetReach() const;
	LineTraceFloorStrategy(AActor* OwnerRef, UWorld* WorldRef);

private:
	const float ConstructDistance = 500.f;

	FCollisionObjectQueryParams FloorTraceObjectParams;
	const ECollisionChannel FLOOR_COLLISION_CHANNEL = ECollisionChannel::ECC_GameTraceChannel1;
	const ECollisionChannel WALL_COLLISION_CHANNEL = ECollisionChannel::ECC_GameTraceChannel2;

};
