
#include "PlayerInteraction.h"
#include "PlayerPick.h"
#include "HoldableObject.h"

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
			if (PlayerPickRef->IsPickbable(Hit)) {
				UE_LOG(LogTemp, Warning, TEXT("YOU CAN GRAB IT"));
				bIsInteractable = true;
			}
			else{
				UE_LOG(LogTemp, Warning, TEXT("YOU CANNOT GRAB IT"));
				bIsInteractable = false;
			}
		}
	}
	else {
		bIsHit = false;
	}
}

// Interact with objects [LSH]
void UPlayerInteraction::Interact(void) {
	UE_LOG(LogTemp, Warning, TEXT("TRY INTERACT"));
	FHitResult Hit = GetFirstPhysicsBodyInReach();
	if (Hit.GetActor()){
		bIsHit = true;
		// If you can grab it, hold it.
		if (Hit.GetActor()->FindComponentByClass<UHoldableObject>()) {
			if (PlayerPickRef->IsPickbable(Hit)) {
				UE_LOG(LogTemp, Warning, TEXT("GRAB"));
				bIsInteractable = true;
				PlayerPickRef->Pick(Hit);
			}
			else {
				bIsInteractable = false;
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