// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerPick.h"
#include "PlayerInteraction.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_API UPlayerInteraction : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerInteraction();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	bool bIsHit = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	bool bIsInteractable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsConstructable = false;

	bool bIsConstructing = false;

private:
	UPlayerPick* PlayerPickRef;
	FHitResult GetObjectInReach() const;
	FHitResult GetFloorInReach() const;
	FVector GetPlayersReach() const;
	FVector GetPlayerLocation() const;
	FRotator GetPlayerRotation() const;
	FCollisionObjectQueryParams FloorTraceObjectParams;

	void Interact();
	void IsInteractable();
	void IsConstructable();

	const float HandDistance = 170.f;
	const float ConstructDistance = 500.f;

	const ECollisionChannel FLOOR_COLLISION_CHANNEL = ECollisionChannel::ECC_GameTraceChannel1;
	const ECollisionChannel WALL_COLLISION_CHANNEL = ECollisionChannel::ECC_GameTraceChannel2;
};
