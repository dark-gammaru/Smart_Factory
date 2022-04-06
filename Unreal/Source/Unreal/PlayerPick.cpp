
#include "PlayerPick.h"
#include "Kismet/GameplayStatics.h"
#include "Commodity.h"
#include "HoldableObject.h"

UPlayerPick::UPlayerPick()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerPick::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	HoldRightHand();
}

void UPlayerPick::BeginPlay()
{
	Super::BeginPlay();

	// Set left hand position ref
	USceneComponent* CubeHolder = Cast<USceneComponent>(GetOwner()->GetDefaultSubobjectByName(TEXT("PositionHolder")));
	CubeHolder->GetChildrenComponents(true, HoldPositions); 

	UInputComponent* InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	InputComponent->BindAxis(TEXT("ObjectSwap"), this, &UPlayerPick::Swap);

	// Get Player Reference for Pick function
	PlayerCharacterRef = GetWorld()->GetFirstPlayerController()->GetPawn();

	HoldingObjectArray.Init(nullptr, 5);
}

/// <summary>
/// Pick a target object, set position and rotation to hand. [LSH]
/// </summary>
/// <param name="Hit"> : Raycast result, Get target object's information from it. </param>
void UPlayerPick::Pick(FHitResult Hit) {
	// Prevent error
	if (HoldingObjectCount >= HoldPositions.Num()) {
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
	ComponentToPick->SetActorRelativeLocation(HoldPositions[HoldingObjectCount]->GetRelativeLocation());
	ComponentToPick->SetActorRelativeRotation(FQuat::Identity);

	HoldingObjectArray[HoldingObjectCount] = ComponentToPick;

	++HoldingObjectCount;
}

/// <summary>
/// Swap right hand object (Object to use for interact).
/// </summary>
/// <param name="MouseAxis"> : Mouse wheel movement.</param>
void UPlayerPick::Swap(float MouseAxis) {
	if (MouseAxis > 0.9f || MouseAxis < -0.9f) {
		if (bIsSwappable) {
			int32 index;
			if (MouseAxis > 0.9f) {
				index = (SelectedIndex + 1) % HoldPositions.Num();
			}
			else {
				index = (SelectedIndex + HoldPositions.Num() - 1) % HoldPositions.Num();
			}
			for (int32 i = 0; i < HoldingObjectArray.Num(); i++) {
				if (HoldingObjectArray[index]) {
					if (HoldingObjectArray[index]->FindComponentByClass<UCommodity>()->bIsAnimal == bSwapIsAnimal
						&& HoldingObjectArray[index]->FindComponentByClass<UCommodity>()->Habitat == SwapHabitat) {
						SelectedIndex = index;
						RightHandObject = HoldingObjectArray[index];
						ReorderObjects();
						UE_LOG(LogTemp, Warning, TEXT("Swap successed"));
						return;
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
	}
}

/// <summary>
/// Check if player could grab target object. [LSH]
/// </summary>
/// <param name="Hit"> : Raycast result, Get target object's weight from it. </param>
/// <returns>True if player can grab object.</returns>
bool UPlayerPick::IsPickable(FHitResult Hit) {
	int32 ObjectWeight = Hit.GetActor()->FindComponentByClass<UHoldableObject>()->GetWeight();
	return MaxWeight >= CurrentWeight + ObjectWeight;
}

/// <summary>
/// Check if player could Interact with certain incubator. 
/// Set proper object's position to right hand. [LSH]
/// </summary>
/// <param name="bIsAnimal"> : Whether the incubator is for animal or not.</param>
/// <param name="IncubatorHabitat"> : Habitat of target incubator.</param>
/// <returns>True if player can interact with target incubator.</returns>
bool UPlayerPick::IsInteractable(bool bIsAnimal, EHabitat IncubatorHabitat) {
	if (bIsSwappable) {
		return true;
	}
	for (int32 i = 0; i < HoldingObjectArray.Num(); ++i) {
		if (HoldingObjectArray[i]) {
			if (HoldingObjectArray[i]->FindComponentByClass<UCommodity>()->bIsAnimal == bIsAnimal
				&& HoldingObjectArray[i]->FindComponentByClass<UCommodity>()->Habitat == IncubatorHabitat) {
				bIsSwappable = true;
				bSwapIsAnimal = bIsAnimal;
				SwapHabitat = IncubatorHabitat;
				SelectedIndex = i;
				RightHandObject = HoldingObjectArray[i];
				return true;
			}
		}
	}
	return false;
}

/// <summary>
/// Hold object with right hand if needed. [LSH]
/// </summary>
void UPlayerPick::HoldRightHand() {
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
/// Reset all values for swap, disable right hand. 
/// Called when raycast hits nothing. [LSH]
/// </summary>
void UPlayerPick::ResetSwapValues() {
	if (bIsSwappable) {
		bIsSwappable = false;
		ReorderObjects();
	}
	RightHandObject = nullptr;
}

/// <summary>
/// Reorder HoldingObjectArray and set all object position to its correct position. 
/// Used to undo right hand object position. [LSH]
/// </summary>
void UPlayerPick::ReorderObjects() {
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
/// Use right hand object and return reference of it. [LSH]
/// </summary>
/// <returns>Reference of right hand object.</returns>
AActor* UPlayerPick::UseRightHandObject() {
	bIsSwappable = false;

	for (int32 i = 0; i < HoldingObjectArray.Num(); ++i) {
		if (HoldingObjectArray[i] == RightHandObject) {
			HoldingObjectArray[i] = nullptr;
			UE_LOG(LogTemp, Warning, TEXT("Deleted"));
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
