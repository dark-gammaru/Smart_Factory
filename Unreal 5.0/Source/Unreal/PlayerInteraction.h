// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerHand.h"
#include "PlayerLineTrace.h"
#include "GarbageChute.h"
#include "IDeactivator.h"
#include "SmartFactoryGameInstance.h"
#include "LineTraceFloorStrategy.h"
#include "LineTraceObjectStrategy.h"
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

private:
	UPlayerHand* PlayerHandRef;
	UPlayerLineTrace* PlayerLineTraceRef;
	IDeactivator* DeactivateTarget;

	void Interact();
	void IsInteractable();
	void IsConstructable();
	void MoveHologramAway();

	void Deactivate();
	
};
