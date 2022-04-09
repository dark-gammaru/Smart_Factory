// Fill out your copyright notice in the Description page of Project Settings.


#include "Commodity.h"

/// <summary>
/// Get bIsAnimal of commodity. [LSH]
/// </summary>
/// <returns>bool bIsAnimal</returns>
bool UCommodity::IsAnimal() {
	return bIsAnimal;
}

/// <summary>
/// Get EHabitat of commodity. [LSH]
/// </summary>
/// <returns>EHabitat Habitat</returns>
EHabitat UCommodity::GetHabitat() {
	return Habitat;
}

/// <summary>
/// Get GrowthTime of commodity. [LSH]
/// </summary>
/// <returns>int32 GrowthTime</returns>
int32 UCommodity::GetGrowthTime() {
	return GrowthTime;
}