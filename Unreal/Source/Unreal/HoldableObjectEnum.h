// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class UNREAL_API HoldableObjectEnum
{
public:
	HoldableObjectEnum();
	~HoldableObjectEnum();
};

UENUM(BlueprintType)
enum class EHabitat : uint8 {
	Plain = 0 UMETA(DisplayName = "Plain"),
	Desert = 1 UMETA(DisplayName = "Desert"),
	Ocean = 2 UMETA(DisplayName = "Ocean"),
};

UENUM(BlueprintType)
enum class EProductColor : uint8 {
	Red = 0 UMETA(DisplayName = "Red"),
	Blue = 1 UMETA(DisplayName = "Blue"),
	Green = 2 UMETA(DisplayName = "Green"),
};

UENUM(BlueprintType)
enum class EProductCategory : uint8 {
	Meat = 0 UMETA(DisplayName = "Meat"),
	Vegetable = 1 UMETA(DisplayName = "Vegetable"),
	Fruit = 2 UMETA(DisplayName = "Fruit"),
};