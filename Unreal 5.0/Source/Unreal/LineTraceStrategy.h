// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class UNREAL_API LineTraceStrategy
{
	
public:
	LineTraceStrategy(AActor* OwnerRef, UWorld* WorldRef);
	virtual FHitResult GetReach() const { return FHitResult(); };

	FVector GetPlayerLocation() const;
	FRotator GetPlayerRotation() const;
	FVector GetPlayersReach(float) const;

protected:
	AActor* OwnerRef;
	UWorld* WorldRef;

};
