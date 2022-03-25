// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerInteraction.h"
#include "PlayerGrab.h"

// Sets default values for this component's properties
UPlayerGrab::UPlayerGrab()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerGrab::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->GetComponents<UPhysicsHandleComponent>(PhysicsHandles);

	UInputComponent* InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("No InputComponent %s"), *GetOwner()->GetName());
	}
	else {
//		InputComponent->BindAction("Grab", IE_Pressed, this, &UPlayerGrab::Grab);
	}

	PlayerInteractionRef = GetOwner()->FindComponentByClass<UPlayerInteraction>();
}


// Called every frame
void UPlayerGrab::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandles[0]->GrabbedComponent) {
		PhysicsHandles[0]->SetTargetLocation(PlayerInteractionRef->GetPlayersReach(GrabDistance));
	}
	// ...
}

void UPlayerGrab::Grab(FHitResult Hit, FVector PlayerReach) {
	UPrimitiveComponent* ComponentToGrab = Hit.GetComponent();

	if (Hit.GetActor()) {
		PhysicsHandles[0]->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None,
			PlayerReach
		);
		UE_LOG(LogTemp, Warning, TEXT("GRABBED"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("NULL"));
	}
}

