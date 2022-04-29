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
	const float ObjectDistance = 200.f;
	ETraceTypeQuery FurnatureTraceChannel = ETraceTypeQuery::TraceTypeQuery1;
};
