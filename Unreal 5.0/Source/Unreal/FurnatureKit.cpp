// Fill out your copyright notice in the Description page of Project Settings.

#include "FurnatureKit.h"
#include "PlayerHand.h"

void UFurnatureKit::SpawnHologram() {
	HologramFurnature = GetWorld()->SpawnActor(HologramFurnatureBP);
}

// Destroy hologram object. [LSH]
void UFurnatureKit::DestroyHologram() {
	if (HologramFurnature) {
		HologramFurnature->Destroy();
		HologramFurnature = nullptr;
	}
}

/// <summary>
/// Set hologram position. [LSH]
/// </summary>
/// <param name="HitLocation">Raycast hit position on floor.</param>
/// <param name="Rotator">FRotator of player rotation.</param>
void UFurnatureKit::SetHologramPosition(FVector HitLocation, FRotator Rotator) {
	if (HologramFurnature) {
		// You must disable physics in order to change actor's location.
		HologramFurnature->DisableComponentsSimulatePhysics();
		HologramFurnature->SetActorLocationAndRotation(HitLocation + FVector(0, 0, 1), FRotator(0., Rotator.Yaw + 180., 0.), false, nullptr, ETeleportType::TeleportPhysics);
		HologramFurnature->FindComponentByClass<UStaticMeshComponent>()->SetSimulatePhysics(true);
	}
}


/// <summary>
/// Destroy hologram furnature and spawn new furniture to hologram's position. [LSH]
/// </summary>
void UFurnatureKit::SpawnFurnature() {
	// Save location and rotation of furnature, and destroy furnature actor.
	FVector FurnatureLocation = HologramFurnature->GetActorLocation();
	FRotator FurnatureRotation = HologramFurnature->GetActorRotation();

	DestroyHologram();

	// Spawn new furnature and set location and rotation.
	AActor* SpawnedFurnature = GetWorld()->SpawnActor(FurnatureBP);
	SpawnedFurnature->GetRootComponent()->SetMobility(EComponentMobility::Movable);
	SpawnedFurnature->SetActorLocationAndRotation(FurnatureLocation, FurnatureRotation);
	SpawnedFurnature->GetRootComponent()->SetMobility(EComponentMobility::Static);

	// Destroy furnature kit.
	GetOwner()->Destroy();
}