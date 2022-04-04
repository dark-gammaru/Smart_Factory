
#include "PlayerInteraction.h"
#include "PlayerPick.h"
#include "HoldableObject.h"
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
}

void UPlayerInteraction::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	IsInteractable();
}

// Check if an object is interactable and change GUI [LSH]
void UPlayerInteraction::IsInteractable(void) {
	FHitResult Hit = GetFirstPhysicsBodyInReach();
	if (Hit.GetActor()) {
		bIsHit = true;
		if (Hit.GetActor()->FindComponentByClass<UHoldableObject>()) {
			if (PlayerPickRef->IsPickable(Hit)) {
				bIsInteractable = true;
			}
			else{
				bIsInteractable = false;
			}
		}
		else if (Hit.GetActor()->FindComponentByClass<UIncubator>()) {
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
		bIsHit = false;
		PlayerPickRef->ResetSwapValues();
	}
}

// Interact with objects [LSH]
void UPlayerInteraction::Interact(void) {
	FHitResult Hit = GetFirstPhysicsBodyInReach();
	if (Hit.GetActor()){
		bIsHit = true;
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
					UE_LOG(LogTemp, Warning, TEXT("Put Commodity"));
				}
				else {
					IncubatorRef->OpenUI();
					UE_LOG(LogTemp, Warning, TEXT("Open UI"));
				}
			}
			// If not, open incubator UI.
			else {
				IncubatorRef->OpenUI();
			}
		}
		else{
			bIsInteractable = false;
		}
	}
	else{
		bIsHit = false;
	}
}

/// <summary>
/// Get player camera position and return it. [LSH]
/// </summary>
/// <returns>FVector of player camera position</returns>
FVector UPlayerInteraction::GetPlayersWorldPos() const {
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
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * HandDistance;
}

/// <summary>
/// Raycast with FVector from GetPlayersWorldPos() and GetPlayersReach() [LSH]
/// </summary>
/// <returns>Raycast result FHitResult</returns>
FHitResult UPlayerInteraction::GetFirstPhysicsBodyInReach() const {
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayersWorldPos(),
		GetPlayersReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
	return Hit;
}