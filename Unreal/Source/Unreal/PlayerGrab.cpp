

#include "PlayerGrab.h"
#include "HoldableObject.h"

UPlayerGrab::UPlayerGrab()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UPlayerGrab::BeginPlay()
{
	Super::BeginPlay();

	USceneComponent* CubeHolder = Cast<USceneComponent>(GetOwner()->GetDefaultSubobjectByName(TEXT("PositionHolder")));
	CubeHolder->GetChildrenComponents(true, HoldPositions);

	// Get Player Reference for Grab function
	PlayerCharacterRef = GetWorld()->GetFirstPlayerController()->GetPawn();
	UE_LOG(LogTemp, Warning, TEXT("%s"), *PlayerCharacterRef->GetName());
}

/// <summary>
/// Grab a target object, set position and rotation to hand. [LSH]
/// </summary>
/// <param name="Hit"> : Raycast result, Get target object's information from it. </param>
void UPlayerGrab::Grab(FHitResult Hit) {
	// Prevent error
	if (HoldingObjects >= HoldPositions.Num()) {
		UE_LOG(LogTemp, Error, TEXT("Grab Error : HoldingObjects >= Hold Max size"));
		return;
	}

	int32 ObjectWeight = Hit.GetActor()->FindComponentByClass<UHoldableObject>()->GetWeight();
	CurrentWeight += ObjectWeight;

	AActor* ComponentToGrab = Hit.GetActor();

	// Disable physics to move object
	ComponentToGrab->DisableComponentsSimulatePhysics();

	// Attatch to character's hold position, set location and rotation
	ComponentToGrab->AttachToActor(PlayerCharacterRef, FAttachmentTransformRules::KeepRelativeTransform);
	ComponentToGrab->SetActorRelativeLocation(HoldPositions[HoldingObjects]->GetRelativeLocation());
	ComponentToGrab->SetActorRelativeRotation(FQuat::Identity);

	++HoldingObjects;
}

/// <summary>
/// Check if player could grab target object. [LSH]
/// </summary>
/// <param name="Hit"> : Raycast result, Get target object's weight from it. </param>
/// <returns>True if player can grab object.</returns>
bool UPlayerGrab::IsGrabbable(FHitResult Hit) {
	int32 ObjectWeight = Hit.GetActor()->FindComponentByClass<UHoldableObject>()->GetWeight();
	return MaxWeight >= CurrentWeight + ObjectWeight;
}

