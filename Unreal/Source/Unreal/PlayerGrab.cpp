

#include "PlayerGrab.h"
#include "HoldableObject.h"
#include "PlayerInteraction.h"

UPlayerGrab::UPlayerGrab()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UPlayerGrab::BeginPlay()
{
	Super::BeginPlay();

	USceneComponent* CubeHolder = Cast<USceneComponent>(GetOwner()->GetDefaultSubobjectByName(TEXT("PositionHolder")));
	CubeHolder->GetChildrenComponents(true, HoldPositions);

	// Get Player Reference for Grab function
	PlayerCharacterRef = GetWorld()->GetFirstPlayerController()->GetPawn();
	UE_LOG(LogTemp, Warning, TEXT("%s"), *PlayerCharacterRef->GetName());
}


void UPlayerGrab::Grab(FHitResult Hit, FVector PlayerReach) {
	if (HoldingObjects >= HoldPositions.Num()) {
		UE_LOG(LogTemp, Error, TEXT("Grab Error : HoldingObjects >= Hold Max size"));
		return;
	}

	int32 ObjectWeight = Hit.GetActor()->FindComponentByClass<UHoldableObject>()->GetWeight();
	CurrentWeight += ObjectWeight;

	AActor* ComponentToGrab = Hit.GetActor();

	ComponentToGrab->DisableComponentsSimulatePhysics();

	ComponentToGrab->AttachToActor(PlayerCharacterRef, FAttachmentTransformRules::KeepRelativeTransform);
	ComponentToGrab->SetActorRelativeLocation(HoldPositions[HoldingObjects]->GetRelativeLocation());
	ComponentToGrab->SetActorRelativeRotation(FQuat::Identity);

	++HoldingObjects;
}

bool UPlayerGrab::IsGrabbable(FHitResult Hit) {
	int32 ObjectWeight = Hit.GetActor()->FindComponentByClass<UHoldableObject>()->GetWeight();
	return MaxWeight >= CurrentWeight + ObjectWeight;
}

