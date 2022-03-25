// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "PlayerGrab.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_API UPlayerGrab : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerGrab();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	TArray<UPhysicsHandleComponent*> PhysicsHandles;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void Grab(FHitResult, FVector);

private:
	class UPlayerInteraction* PlayerInteractionRef;

	int32 CurrentWeight;
	const int32 MaxWeight = 5;

	const float GrabDistance = 50.f;
	
};
