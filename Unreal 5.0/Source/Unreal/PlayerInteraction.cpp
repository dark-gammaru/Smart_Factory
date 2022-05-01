

#include "PlayerInteraction.h"
#include "HoldableObject.h"
#include "FurnatureKit.h"
#include "GarbageChute.h"
#include "Bed.h"
#include "PlayerLineTrace.h"
#include "Engine/StaticMeshActor.h"
#include "Incubator.h"

UPlayerInteraction::UPlayerInteraction()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerInteraction::BeginPlay()
{
	Super::BeginPlay();

	UInputComponent* InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	InputComponent->BindAction(TEXT("Click"), IE_Pressed, this, &UPlayerInteraction::Interact);

	PlayerHandRef = GetOwner()->FindComponentByClass<UPlayerHand>();
	PlayerLineTraceRef = GetOwner()->FindComponentByClass<UPlayerLineTrace>();
}

void UPlayerInteraction::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PlayerLineTraceRef->bIsConstructing) {
		IsConstructable();
	}
	else {
		IsInteractable();
	}
}


// Check if player can construct and change GUI. [LSH]
void UPlayerInteraction::IsConstructable() {
	FHitResult Hit = PlayerLineTraceRef->GetHitResult();
	bIsHit = false;
	if (auto TargetActor = Hit.GetActor()) {
		// If hit target is floor, move hologram to hit position.
		// It is interactable if hologram is not overlapping with anything.
		if (TargetActor->ActorHasTag("Floor")) {
			bIsHit = true;
			PlayerHandRef->GetRightHand()->FindComponentByClass<UFurnatureKit>()
				->SetHologramPosition(Hit.Location, PlayerLineTraceRef->GetPlayerRotation());
			bIsInteractable = bIsConstructable;
		}
		// If hit target is garbage chute, activate chute and prepare to sell a kit.
		else if (auto HitGarbageChute = TargetActor->FindComponentByClass<UGarbageChute>()) {
			bIsHit = true;
			auto ResultTuple = PlayerHandRef->IsInteractableGarbageChute();
			bIsInteractable = ResultTuple.Key;
			HitGarbageChute->Activate(ResultTuple.Value);
			DeactivateTarget = HitGarbageChute; 
			MoveHologramAway();
		}
		// If hit target is not floor nor garbage chute, set cursor disabled.
		else {
			MoveHologramAway();
			bIsInteractable = false;
		}
	}
	else {
		MoveHologramAway();
		bIsInteractable = false;
	}
}

// Check if an object is interactable and change GUI. [LSH]
void UPlayerInteraction::IsInteractable() {
	FHitResult Hit = PlayerLineTraceRef->GetHitResult();
	bIsHit = false;
	if (auto HitActor = Hit.GetActor()) {
		if (auto HitHoldableObject = HitActor->FindComponentByClass<UHoldableObject>()) {
			bIsHit = true;
			bIsInteractable = PlayerHandRef->IsHoldable(HitHoldableObject);
		}
		else if (auto HitIncubator = HitActor->FindComponentByClass<UIncubator>()) {
			bIsHit = true;
			bIsInteractable = PlayerHandRef->IsInteractableIncubator(HitIncubator);
		}
		else if (auto HitGarbageChute = HitActor->FindComponentByClass<UGarbageChute>()) {
			bIsHit = true;
			auto ResultTuple = PlayerHandRef->IsInteractableGarbageChute();
			bIsInteractable = ResultTuple.Key;
			if (ResultTuple.Key) {
				HitGarbageChute->Activate(ResultTuple.Value);
				DeactivateTarget = HitGarbageChute;
			}
			else {
				bIsInteractable = false;
				Deactivate();
			}
		}
		else if (auto HitBed = HitActor->FindComponentByClass<UBed>()){
			bIsHit = true;
			bIsInteractable = true;
		}
		else {
			PlayerHandRef->ResetSwapValues();
			Deactivate();
		}
	}
	else {
		PlayerHandRef->ResetSwapValues();
		Deactivate();
	}
}

// Interact with objects. [LSH]
void UPlayerInteraction::Interact() {
	FHitResult Hit = PlayerLineTraceRef->GetHitResult();
	if (auto HitActor = Hit.GetActor()) {
		// If you are constructing, check validity and construct.
		if (PlayerLineTraceRef->bIsConstructing) {
			// If hit target is garbage chute, sell furnature kit.
			if (auto HitGarbageChute = HitActor->FindComponentByClass<UGarbageChute>()) {
				if (PlayerHandRef->IsInteractableGarbageChute().Key) {
					auto PlayerKit = PlayerHandRef->UseRightHand();
					PlayerKit->FindComponentByClass<UFurnatureKit>()->DestroyHologram();
					HitGarbageChute->SellObject(PlayerKit);
					PlayerLineTraceRef->bIsConstructing = false;
					return;
				}
			}
			// If it is constructable, then spawn furnature.
			else if (bIsConstructable && bIsInteractable) {
				PlayerHandRef->UseRightHand()->FindComponentByClass<UFurnatureKit>()->SpawnFurnature();
				PlayerLineTraceRef->bIsConstructing = false;
			}
		}
		else {
			// If you can grab holdable object, hold it.
			if (auto HitHoldableObject = HitActor->FindComponentByClass<UHoldableObject>()) {
				if (PlayerHandRef->IsHoldable(HitHoldableObject)) {
					UE_LOG(LogTemp, Warning, TEXT("GRAB"));
					PlayerHandRef->Hold(HitActor);
				}
			}
			// If you have appropreate commodity, start growing commodity immediately.
			else if (auto HitIncubator = HitActor->FindComponentByClass<UIncubator>()) {
				if (PlayerHandRef->IsInteractableIncubator(HitIncubator)) {
					HitIncubator->PutCommodity(PlayerHandRef->UseRightHand());
				}
			}
			// If hit target is garbage chute, sell holding object.
			else if (auto HitGarbageChute = HitActor->FindComponentByClass<UGarbageChute>()) {
				if (PlayerHandRef->IsInteractableGarbageChute().Key) {
					HitGarbageChute->SellObject(PlayerHandRef->UseRightHand());
				}
			}
			else if (auto HitBed = HitActor->FindComponentByClass<UBed>()) {
				HitBed->isClicked = true;
			}
		}
	}
}

// Hide hologram by moving it far away. [LSH]
void UPlayerInteraction::MoveHologramAway() {
	if (auto HandObject = PlayerHandRef->GetRightHand()) {
		if (auto TargetKit = HandObject->FindComponentByClass<UFurnatureKit>()) {
			TargetKit->SetHologramPosition(FVector(0., -1000., 0.), FRotator::ZeroRotator);
		}
	}
}

// Deactivate if there is activated object. [LSH]
void UPlayerInteraction::Deactivate() {
	if (DeactivateTarget) {
		DeactivateTarget->Deactivate();
		DeactivateTarget = nullptr;
	}
}

