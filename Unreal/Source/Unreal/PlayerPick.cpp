

#include "PlayerPick.h"
#include "HoldableObject.h"

UPlayerPick::UPlayerPick()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UPlayerPick::BeginPlay()
{
	Super::BeginPlay();

	USceneComponent* CubeHolder = Cast<USceneComponent>(GetOwner()->GetDefaultSubobjectByName(TEXT("PositionHolder")));
	CubeHolder->GetChildrenComponents(true, HoldPositions);

	// Get Player Reference for Pick function
	PlayerCharacterRef = GetWorld()->GetFirstPlayerController()->GetPawn();
	UE_LOG(LogTemp, Warning, TEXT("%s"), *PlayerCharacterRef->GetName());
}

/// <summary>
/// Pick a target object, set position and rotation to hand. [LSH]
/// </summary>
/// <param name="Hit"> : Raycast result, Get target object's information from it. </param>
void UPlayerPick::Pick(FHitResult Hit) {
	// Prevent error
	if (HoldingObjects >= HoldPositions.Num()) {
		UE_LOG(LogTemp, Error, TEXT("Pick Error : HoldingObjects >= Hold Max size"));
		return;
	}

	int32 ObjectWeight = Hit.GetActor()->FindComponentByClass<UHoldableObject>()->GetWeight();
	CurrentWeight += ObjectWeight;

	AActor* ComponentToPick = Hit.GetActor();

	// Disable physics to move object
	ComponentToPick->DisableComponentsSimulatePhysics();

	// Attatch to character's hold position, set location and rotation
	ComponentToPick->AttachToActor(PlayerCharacterRef, FAttachmentTransformRules::KeepRelativeTransform);
	ComponentToPick->SetActorRelativeLocation(HoldPositions[HoldingObjects]->GetRelativeLocation());
	ComponentToPick->SetActorRelativeRotation(FQuat::Identity);

	++HoldingObjects;
}

/// <summary>
/// Check if player could grab target object. [LSH]
/// </summary>
/// <param name="Hit"> : Raycast result, Get target object's weight from it. </param>
/// <returns>True if player can grab object.</returns>
bool UPlayerPick::IsPickbable(FHitResult Hit) {
	int32 ObjectWeight = Hit.GetActor()->FindComponentByClass<UHoldableObject>()->GetWeight();
	return MaxWeight >= CurrentWeight + ObjectWeight;
}

