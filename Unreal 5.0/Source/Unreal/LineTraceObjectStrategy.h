// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LineTraceStrategy.h"

/**
 * 
 */
class UNREAL_API LineTraceObjectStrategy : public LineTraceStrategy
{
	
public:
	FHitResult GetReach() const;
	LineTraceObjectStrategy(AActor* OwnerRef, UWorld* WorldRef);

private:
	const float ObjectDistance = 170.f;
	ETraceTypeQuery ObjectTraceChannel = ETraceTypeQuery::TraceTypeQuery2;
	
};
