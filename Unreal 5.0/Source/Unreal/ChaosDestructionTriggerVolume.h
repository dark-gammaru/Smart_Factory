// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "ChaosDestructable.h"
#include "ChaosDestructionTriggerVolume.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREAL_API AChaosDestructionTriggerVolume : public ATriggerVolume
{
	GENERATED_BODY()

public:
	AChaosDestructionTriggerVolume();

	UFUNCTION(BlueprintCallable)
	void MakeDestructable();

	void ChangeSize(int32 Phase);

private:
	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor);

	UPROPERTY()
	TArray<UChaosDestructable*> DestructionTargets;
	TArray<FVector> PhaseSize{ FVector(1.f, 50.f, 4.f), FVector(50.f, 1.f, 4.f) , FVector(2.f, 50.f, 4.f) };
};
