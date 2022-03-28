// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerInteraction.h"
#include "PlayerGrab.h"
#include "HoldableObject.h"

// Sets default values for this component's properties
UPlayerInteraction::UPlayerInteraction()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerInteraction::BeginPlay()
{
	Super::BeginPlay();

	UInputComponent* InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	InputComponent->BindAction(TEXT("Click"), IE_Pressed, this, &UPlayerInteraction::Interact);

	PlayerGrabRef = GetOwner()->FindComponentByClass<UPlayerGrab>();
}


// Called every frame
void UPlayerInteraction::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	IsInteractable();
}

void UPlayerInteraction::IsInteractable(void) {
	FHitResult Hit = GetFirstPhysicsBodyInReach();
	if (Hit.GetActor()) {
		if (Hit.GetActor()->FindComponentByClass<UHoldableObject>()) {
			if (PlayerGrabRef->IsGrabbable(Hit)) {
				UE_LOG(LogTemp, Warning, TEXT("YOU CAN GRAB IT"));
			}
			else{
				UE_LOG(LogTemp, Warning, TEXT("YOU CANNOT GRAB IT"));
			}
		}
	}
}

void UPlayerInteraction::Interact(void) {
	UE_LOG(LogTemp, Warning, TEXT("TRY INTERACT"));
	FHitResult Hit = GetFirstPhysicsBodyInReach();
	if (Hit.GetActor()){
		// If you can grab it, hold it.
		if (Hit.GetActor()->FindComponentByClass<UHoldableObject>()) {
			if (PlayerGrabRef->IsGrabbable(Hit)) {
				UE_LOG(LogTemp, Warning, TEXT("GRAB"));
				PlayerGrabRef->Grab(Hit, GetPlayersReach(HandDistance));
			}
		}
	}
}

FHitResult UPlayerInteraction::GetFirstPhysicsBodyInReach() const {
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayersWorldPos(),
		GetPlayersReach(HandDistance),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
	return Hit;
}


FVector UPlayerInteraction::GetPlayersReach(float Distance) const {
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Distance;
}

FVector UPlayerInteraction::GetPlayersWorldPos() const {
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation;
}
