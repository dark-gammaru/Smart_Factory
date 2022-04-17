

#include "PlayerInteraction.h"
#include "HoldableObject.h"
#include "FurnatureKit.h"
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
	bIsHit = true;
	if (Hit.GetActor()) {
		if (Hit.GetActor()->ActorHasTag("Floor")) {
			PlayerHandRef->SetRightHand();
			PlayerHandRef->GetRightHand()->FindComponentByClass<UFurnatureKit>()
				->SetHologramPosition(Hit.Location, PlayerLineTraceRef->GetPlayerRotation());
			bIsInteractable = true;
		}
	}
	else {
		
		Hit = PlayerLineTraceRef->ForceLineTraceObject();

		if (Hit.GetActor()) {
			// TODO : Check dust chute : change interactable to true
		}
		
		PlayerHandRef->GetRightHand()->FindComponentByClass<UFurnatureKit>()
			->SetHologramPosition(FVector(0., -1000., 0.), PlayerLineTraceRef->GetPlayerRotation());
		PlayerHandRef->ResetSwapValues();
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
			if (PlayerHandRef->IsHoldable(HitHoldableObject)) {
				bIsInteractable = true;
			}
			else {
				bIsInteractable = false;
			}
		}
		else if (auto HitIncubator = Hit.GetActor()->FindComponentByClass<UIncubator>()) {
			bIsHit = true;
			// If you have appropreate commodity, hold it with right hand
			if (HitIncubator->IsEmpty()) {
				PlayerHandRef->IsInteractable(HitIncubator);
			}
			// Player can always interact with incubator.
			bIsInteractable = true;
		}
		else {
			bIsInteractable = false;
		}
	}
	else {
		PlayerHandRef->ResetSwapValues();
	}
}

// Interact with objects. [LSH]
void UPlayerInteraction::Interact() {
	FHitResult Hit = PlayerLineTraceRef->GetHitResult();
	if (auto HitActor = Hit.GetActor()) {
		bIsHit = true;
		// If you are constructing, check validity and construct.
		if (PlayerLineTraceRef->bIsConstructing) {
			if (bIsConstructable && bIsInteractable) {
				auto PlayerKit = PlayerHandRef->UseRightHand();
				PlayerKit->FindComponentByClass<UFurnatureKit>()->SpawnFurnature();
				bIsConstructable = false;
				PlayerLineTraceRef->bIsConstructing = false;
			}
		}
		else {
			// If you can grab it, hold it.
			if (auto HitHoldableObject = HitActor->FindComponentByClass<UHoldableObject>()) {
				if (PlayerHandRef->IsHoldable(HitHoldableObject)) {
					UE_LOG(LogTemp, Warning, TEXT("GRAB"));
					bIsInteractable = true;
					PlayerHandRef->Hold(HitActor);
				}
				else {
					bIsInteractable = false;
				}
			}
			else if (auto HitIncubator = HitActor->FindComponentByClass<UIncubator>()) {
				// Player can always interact with incubator.
				bIsInteractable = true;
				// If you have appropreate commodity, start growing commodity immediately.
				if (HitIncubator->IsEmpty()) {
					if (PlayerHandRef->IsInteractable(HitIncubator)) {
						HitIncubator->PutCommodity(PlayerHandRef->UseRightHand());
					}
					else {
						HitIncubator->OpenUI();
					}
				}
				// If not, open incubator UI.
				else {
					HitIncubator->OpenUI();
				}
			}
			else {
				bIsInteractable = false;
			}
		}
	}
	else {
		bIsHit = false;
	}
}



