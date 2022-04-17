// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerHand.h"
#include "Commodity.h"
#include "HoldableObject.h"
#include "FurnatureKit.h"
#include "PlayerInteraction.h"

UPlayerHand::UPlayerHand()
{
	PrimaryComponentTick.bCanEverTick = true;
	HoldingObjectArray.Init(nullptr, 5);
}

// Called when the game starts
void UPlayerHand::BeginPlay()
{
	Super::BeginPlay();

	// Set left hand position ref
	USceneComponent* CubeHolder = Cast<USceneComponent>(GetOwner()->GetDefaultSubobjectByName(TEXT("PositionHolder")));
	CubeHolder->GetChildrenComponents(true, HoldPositions);

	UInputComponent* InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	InputComponent->BindAxis(TEXT("ObjectSwap"), this, &UPlayerHand::Swap);

	// Get Player Reference for Pick function
	PlayerCharacterRef = GetWorld()->GetFirstPlayerController()->GetPawn();
}

// Called every frame
void UPlayerHand::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	HoldRightHand();
}

/// <summary>
/// Check if player can hold a holdableobject. [LSH]
/// </summary>
/// <param name="Target">HoldableObject of target.</param>
/// <returns></returns>
bool UPlayerHand::IsHoldable(UHoldableObject* Target) {
	if (HoldingObjectCount >= HoldPositions.Num()) {
		UE_LOG(LogTemp, Error, TEXT("Hold Error : HoldingObjects >= Hold Max size"));
		return false;
	}

	if (Target->GetWeight() + CurrentWeight > MaxWeight) {
		return false;
	}

	return true;
}

/// <summary>
/// Pick a target object, set position and rotation to hand. [LSH]
/// </summary>
/// <param name="Hit"> : Raycast result, Get target object's information from it. </param>
void UPlayerHand::Hold(AActor* Target) {
	int32 ObjectWeight = Target->FindComponentByClass<UHoldableObject>()->GetWeight();
	CurrentWeight += ObjectWeight;

	// Disable physics to move object
	Target->DisableComponentsSimulatePhysics();

	// Attatch to character's hold position, set location and rotation
	Target->AttachToActor(PlayerCharacterRef, FAttachmentTransformRules::KeepRelativeTransform);
	Target->SetActorRelativeLocation(HoldPositions[HoldingObjectCount]->GetRelativeLocation());
	Target->SetActorRelativeRotation(FQuat::Identity);

	HoldingObjectArray[HoldingObjectCount] = Target;

	++HoldingObjectCount;

	if (auto TargetKit = Target->FindComponentByClass<UFurnatureKit>()) {
		TargetKit->SpawnHologram();
		PlayerCharacterRef->FindComponentByClass<UPlayerLineTrace>()->bIsConstructing = true;
	}
}

/// <summary>
/// Hold object with right hand if needed. [LSH]
/// </summary>
void UPlayerHand::HoldRightHand() {
	if (RightHandObject == nullptr) {
		return;
	}

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	// Get player camera's position and rotation
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	RightHandObject->SetActorLocation(PlayerViewPointLocation +
		PlayerViewPointRotation.Vector() * 100.f +								// move front direction
		PlayerViewPointRotation.RotateVector(FVector(0.f, 90.f, 0.f)) * 0.6f);  // move right direction
}

/// <summary>
/// Swap right hand object (Object to use for interact). [LSH]
/// </summary>
/// <param name="MouseAxis"> : Mouse wheel movement.</param>
void UPlayerHand::Swap(float MouseAxis) {
	if (MouseAxis > 0.9f || MouseAxis < -0.9f) {
		if (RightHandObject) {
			if (CurrentIncubator) {
				int32 index;
				if (MouseAxis > 0.9f) {
					index = (SelectedIndex + 1) % HoldPositions.Num();
				}
				else {
					index = (SelectedIndex + HoldPositions.Num() - 1) % HoldPositions.Num();
				}
				for (int32 i = 0; i < HoldingObjectArray.Num(); i++) {
					if (HoldingObjectArray[index]) {
						if (auto CommodityRef = HoldingObjectArray[index]->FindComponentByClass<UCommodity>()) {
							if (CurrentIncubator->IsInteractable(CommodityRef)) {
								SelectedIndex = index;
								RightHandObject = HoldingObjectArray[index];
								ReorderObjects();
								UE_LOG(LogTemp, Warning, TEXT("Swap successed"));
								return;
							}
						}
					}
					if (MouseAxis > 0.9f) {
						index = (index + 1) % HoldPositions.Num();
					}
					else {
						index = (index + HoldPositions.Num() - 1) % HoldPositions.Num();
					}
				}
			}
			// TODO : if dust scute
		}
	}
}

/// <summary>
/// Reorder HoldingObjectArray and set all object position to its correct position. 
/// Used to undo right hand object position. [LSH]
/// </summary>
void UPlayerHand::ReorderObjects() {
	for (int32 i = 0; i < HoldingObjectArray.Num() - 1; ++i) {
		if (HoldingObjectArray[i] == nullptr) {
			HoldingObjectArray[i] = HoldingObjectArray[i + 1];
			HoldingObjectArray[i + 1] = nullptr;
		}
	}

	// Change object position after moving pointer.
	for (int32 i = 0; i < HoldingObjectArray.Num(); ++i) {
		if (HoldingObjectArray[i] != nullptr) {
			HoldingObjectArray[i]->SetActorRelativeLocation(HoldPositions[i]->GetRelativeLocation());
			HoldingObjectArray[i]->SetActorRelativeRotation(FQuat::Identity);
		}
	}
}


/// <summary>
/// Reset all values for swap, disable right hand. 
/// Called when raycast hits nothing. [LSH]
/// </summary>
void UPlayerHand::ResetSwapValues() {
	if (RightHandObject) {
		RightHandObject = nullptr;
		ReorderObjects();
	}
	CurrentIncubator = nullptr;
}


/// <summary>
/// Check if player could construct a furnature. [LSH]
/// </summary>
/// <returns></returns>
void UPlayerHand::SetRightHand() {
	if (RightHandObject) {
		return;
	}
	for (int32 i = 0; i < HoldingObjectArray.Num(); ++i) {
		if (HoldingObjectArray[i]) {
			if (HoldingObjectArray[i]->FindComponentByClass<UFurnatureKit>()) {
				SelectedIndex = i;
				RightHandObject = HoldingObjectArray[i];
				return;
			}
		}
	}
}


const AActor* UPlayerHand::GetRightHand() {
	return RightHandObject;
}

/// <summary>
/// Use right hand object and return reference of it. [LSH]
/// </summary>
/// <returns>Reference of right hand object.</returns>
AActor* UPlayerHand::UseRightHand() {
	for (int32 i = 0; i < HoldingObjectArray.Num(); ++i) {
		if (HoldingObjectArray[i] == RightHandObject) {
			HoldingObjectArray[i] = nullptr;
			break;
		}
	}

	if (RightHandObject->FindComponentByClass<UHoldableObject>()) {
		CurrentWeight -= RightHandObject->FindComponentByClass<UHoldableObject>()->GetWeight();
	}

	ReorderObjects();

	auto ReturnRef = RightHandObject;
	RightHandObject = nullptr;
	--HoldingObjectCount;
	return ReturnRef;
}

/// <summary>
/// Check if player is interactable with incubator.
/// And set proper right hand object. [LSH]
/// </summary>
/// <param name="TargetIncubator"></param>
/// <returns></returns>
bool UPlayerHand::IsInteractable(UIncubator* TargetIncubator) {
	if (RightHandObject) {
		return true;
	}
	for (int32 i = 0; i < HoldingObjectArray.Num(); ++i) {
		if (HoldingObjectArray[i]) {
			if (auto CommodityRef = HoldingObjectArray[i]->FindComponentByClass<UCommodity>()) {
				if (TargetIncubator->IsInteractable(CommodityRef)) {
					CurrentIncubator = TargetIncubator;
					SelectedIndex = i;
					RightHandObject = HoldingObjectArray[i];
					return true;
				}
			}
		}
	}
	return false;
}