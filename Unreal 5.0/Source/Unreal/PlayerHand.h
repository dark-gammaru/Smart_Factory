// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Incubator.h"
#include "HoldableObjectEnum.h"
#include "PlayerHand.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREAL_API UPlayerHand : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerHand();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool IsHoldable(UHoldableObject*);

	void ResetSwapValues();

	void SetRightHand();

	const AActor* GetRightHand();

	bool IsInteractableIncubator(UIncubator*);

	bool IsInteractableSupplyPort(UIncubator*);

	TTuple<bool, int32> IsInteractableGarbageChute();

	AActor* UseRightHand();

	void Hold(AActor*);

private:
	UPROPERTY()
	APawn* PlayerCharacterRef;

	int32 CurrentWeight;

	const int32 MaxWeight = 5;

	int32 HoldingObjectCount = 0;

	UPROPERTY()
	TArray<USceneComponent*> HoldPositions;

	UPROPERTY()
	TArray<AActor*> HoldingObjectArray;

	UPROPERTY()
	AActor* RightHandObject;

	void HoldRightHand();

	void Swap(float);

	int32 SelectedIndex;

	void ReorderObjects();

	UPROPERTY()
	UIncubator* CurrentIncubator;
};