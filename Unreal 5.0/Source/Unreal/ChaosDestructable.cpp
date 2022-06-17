
#include "ChaosDestructable.h"


/// <summary>
/// Remove all components and call Destruction.
/// </summary>
void UChaosDestructable::MakeDestructable() {
	// Remember Location and Rotation
	FVector Location = GetOwner()->GetActorLocation();
	FRotator Rotation = GetOwner()->GetActorRotation();

	// Remove everything except Incubator component.
	auto ActorComponents = GetOwner()->GetComponentsByClass(UActorComponent::StaticClass());
	for (auto ac : ActorComponents) {
		if (Cast<UChaosDestructable>(ac) == nullptr) {
			ac->DestroyComponent();
		}
	}

	Destruction(Location, Rotation);
}

/// <summary>
/// Swap mesh to GeometryCollection and destroy all after few seconds
/// </summary>
/// <param name="Location"></param>
/// <param name="Rotation"></param>
void UChaosDestructable::Destruction(FVector Location, FRotator Rotation) {
	// Spawn and save in order to delete after few seconds.
	auto GeometryCollection = GetWorld()->SpawnActor(GeometryCollectionBP, &Location, &Rotation);

	GetWorld()->GetTimerManager().SetTimer(DestructionDelayHandle, FTimerDelegate::CreateLambda([=]() {
		GeometryCollection->Destroy();
		GetOwner()->Destroy();
		}), 1.f, false, 7.f);
}