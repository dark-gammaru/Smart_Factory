// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerGrab.h"
#include "PlayerInteraction.h"

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

	// ...
}

void UPlayerInteraction::Interact(void) {
	UE_LOG(LogTemp, Warning, TEXT("INTERACT"));
	FHitResult TempHit = GetFirstPhysicsBodyInReach();
	if (TempHit.GetActor()) {
		PlayerGrabRef->Grab(TempHit, GetPlayersReach(HandDistance));
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
