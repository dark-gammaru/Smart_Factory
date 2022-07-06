// Fill out your copyright notice in the Description page of Project Settings.

#include "ChaosDestructionTriggerVolume.h"

AChaosDestructionTriggerVolume::AChaosDestructionTriggerVolume() {
	OnActorBeginOverlap.AddDynamic(this, &AChaosDestructionTriggerVolume::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &AChaosDestructionTriggerVolume::OnOverlapEnd);
}

/// <summary>
/// If overlapped actor implemented ChaosDestructable, register it to an array. [LSH]
/// </summary>
/// <param name="OverlappedActor"></param>
/// <param name="OtherActor"></param>
void AChaosDestructionTriggerVolume::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor) {
	if (OtherActor && OtherActor != this) {
		auto ActorComponents = OtherActor->GetComponentsByClass(UActorComponent::StaticClass());
		for (auto ActorComponent : ActorComponents) {
			if (auto ChaosComponent = Cast<UChaosDestructable>(ActorComponent)) {
				DestructionTargets.Add(ChaosComponent);
			}
		}
	}
}

/// <summary>
/// If exited actor implemented ChaosDestructable, remove it from an array. [LSH]
/// </summary>
/// <param name="OverlappedActor"></param>
/// <param name="OtherActor"></param>
void AChaosDestructionTriggerVolume::OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor) {
	if (OtherActor && OtherActor != this) {
		auto ActorComponents = OtherActor->GetComponentsByClass(UActorComponent::StaticClass());
		for (auto ActorComponent : ActorComponents) {
			if (auto ChaosComponent = Cast<UChaosDestructable>(ActorComponent)) {
				DestructionTargets.Remove(ChaosComponent);
			}
		}
	}
}

void AChaosDestructionTriggerVolume::ChangeSize(int32 Phase) {
	SetActorScale3D(PhaseSize[Phase]);
	SetActorLocation(FVector(0, 0, -2000.f));
}

void AChaosDestructionTriggerVolume::MakeDestructable() {
	// MakeDestroyable calls OnOverlapEnd and interrupts iterating, so make a copy of DestructionTargets.
	auto DestructArray = TArray<UChaosDestructable*>(DestructionTargets);
	for (UChaosDestructable* Target : DestructArray) {
		if (Target != nullptr) {
			Target->MakeDestructable();
		}
	}
	DestructionTargets.Empty();
}