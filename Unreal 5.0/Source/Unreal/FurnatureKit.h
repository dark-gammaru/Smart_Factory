// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HoldableObject.h"
#include "PlayerLineTrace.h"
#include "FurnatureKit.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREAL_API UFurnatureKit : public UHoldableObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	UBlueprintGeneratedClass* HologramFurnatureBP;

	UPROPERTY(EditAnywhere)
	UBlueprintGeneratedClass* FurnatureBP;

	void SpawnHologram();
	void DestroyHologram();
	void SetHologramPosition(FVector, FRotator);
	void SpawnFurnature();

private:
	AActor* HologramFurnature;
};
