// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HoldableObject.h"
#include "Commodity.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_API UCommodity : public UHoldableObject
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	FString ScientificName;

	UPROPERTY(EditAnywhere)
	FString Information;

	UPROPERTY(EditAnywhere)
	int32 GrowthTime;
};
