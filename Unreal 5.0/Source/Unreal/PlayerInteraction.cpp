
#include "PlayerInteraction.h"
#include "PlayerPick.h"
#include "HoldableObject.h"
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

	PlayerPickRef = GetOwner()->FindComponentByClass<UPlayerPick>();

	// Floor traces only with wall and floor
	FloorTraceObjectParams.AddObjectTypesToQuery(FLOOR_COLLISION_CHANNEL);
	FloorTraceObjectParams.AddObjectTypesToQuery(WALL_COLLISION_CHANNEL);
}

void UPlayerInteraction::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsConstructing) {
		IsConstructable();
	}
	else {
		IsInteractable();
	}
}

// Check if player can construct and change GUI. [LSH]
void UPlayerInteraction::IsConstructable(void) {
	FHitResult Hit = GetFloorInReach();
	bIsHit = true;
	if (Hit.GetActor() && Hit.GetActor()->ActorHasTag("Floor")) {
		PlayerPickRef->SetRightHand();
		PlayerPickRef->SetHologramPosition(Hit.Location, GetPlayerRotation());
		bIsInteractable = true;
	}
	else {
		/*
		Hit = GetFirstObjectInReach();
		if (Hit.GetActor()) {
			// TODO : Check dust chute
		}
		*/
		PlayerPickRef->SetHologramPosition(FVector(0., -1000., 0.), GetPlayerRotation());
		PlayerPickRef->ResetSwapValues();
		bIsInteractable = false;
	}
}

// Check if an object is interactable and change GUI. [LSH]
void UPlayerInteraction::IsInteractable(void) {
	FHitResult Hit = GetObjectInReach();
	bIsHit = false;
	if (Hit.GetActor()) {
		if (Hit.GetActor()->FindComponentByClass<UHoldableObject>()) {
			bIsHit = true;
			if (PlayerPickRef->IsPickable(Hit)) {
				bIsInteractable = true;
			}
			else {
				bIsInteractable = false;
			}
		}
		else if (Hit.GetActor()->FindComponentByClass<UIncubator>()) {
			bIsHit = true;
			auto IncubatorRef = Hit.GetActor()->FindComponentByClass<UIncubator>();
			// If you have appropreate commodity, hold it with right hand
			if (IncubatorRef->IsEmpty()) {
				if (PlayerPickRef->IsInteractable(IncubatorRef->IsAnimal(), IncubatorRef->GetHabitat())) {
				}
			}
			// Player can always interact with incubator.
			bIsInteractable = true;
		}
		else {
			bIsInteractable = false;
		}
	}
	else {
		PlayerPickRef->ResetSwapValues();
	}
}

// Interact with objects. [LSH]
void UPlayerInteraction::Interact(void) {
	FHitResult Hit = GetObjectInReach();
	if (Hit.GetActor()) {
		bIsHit = true;
		// If you are constructing, check validity and construct.
		if (bIsConstructing) {
			if (bIsConstructable && bIsInteractable) {
				PlayerPickRef->ConstructFurnature();
				bIsConstructable = false;
				bIsConstructing = false;
			}
		}
		else {
			// If you can grab it, hold it.
			if (Hit.GetActor()->FindComponentByClass<UHoldableObject>()) {
				if (PlayerPickRef->IsPickable(Hit)) {
					UE_LOG(LogTemp, Warning, TEXT("GRAB"));
					bIsInteractable = true;
					PlayerPickRef->Pick(Hit);
				}
				else {
					bIsInteractable = false;
				}
			}
			else if (Hit.GetActor()->FindComponentByClass<UIncubator>()) {
				auto IncubatorRef = Hit.GetActor()->FindComponentByClass<UIncubator>();
				// Player can always interact with incubator.
				bIsInteractable = true;
				// If you have appropreate commodity, start growing commodity immediately.
				if (IncubatorRef->IsEmpty()) {
					if (PlayerPickRef->IsInteractable(IncubatorRef->IsAnimal(), IncubatorRef->GetHabitat())) {
						IncubatorRef->PutCommodity(PlayerPickRef->UseRightHandObject());
					}
					else {
						IncubatorRef->OpenUI();
					}
				}
				// If not, open incubator UI.
				else {
					IncubatorRef->OpenUI();
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

/// <summary>
/// Get player camera position and return it. [LSH]
/// </summary>
/// <returns>FVector of player camera position</returns>
FVector UPlayerInteraction::GetPlayerLocation() const {
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	// Get player's camera position and rotation
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	// Return player camera's location
	return PlayerViewPointLocation;
}

/// <summary>
/// Get player camera rotation and return it. [LSH]
/// </summary>
/// <returns>FRotator of player camera rotation.</returns>
FRotator UPlayerInteraction::GetPlayerRotation() const {
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	// Get player's camera position and rotation
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	// Return player camera's rotation
	return PlayerViewPointRotation;
}

/// <summary>
/// Calculate raycast end point vector with player viewpoint and float HandDistance [LSH]
/// </summary>
/// <returns>FVector of player camera position to raycast end point</returns>
FVector UPlayerInteraction::GetPlayersReach() const {
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	// Get player camera's position and rotation
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	// Make vector from Rotation, Location, Distance
	if (bIsConstructing) {
		return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * ConstructDistance;
	}
	else {
		return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * HandDistance;
	}
}

/// <summary>
/// Raycast with FVector from GetPlayersWorldPos() and GetPlayersReach() [LSH]
/// </summary>
/// <returns>Raycast result FHitResult</returns>
FHitResult UPlayerInteraction::GetObjectInReach() const {
	FHitResult Hit;
	FCollisionQueryParams ObjectQueryParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayerLocation(),
		GetPlayersReach(),
		FCollisionObjectQueryParams(FCollisionObjectQueryParams::DefaultObjectQueryParam),
		ObjectQueryParams
	);
	return Hit;
}

/// <summary>
/// Only raycast to floor and wall, with FVector from GetPlayersWorldPos() and GetPlayersReach() [LSH]
/// </summary>
/// <returns>Raycast result FHitResult</returns>
FHitResult UPlayerInteraction::GetFloorInReach() const {
	FHitResult Hit;
	FCollisionQueryParams FloorQueryParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayerLocation(),
		GetPlayersReach(),
		FloorTraceObjectParams,
		FloorQueryParams
	);
	return Hit;
}