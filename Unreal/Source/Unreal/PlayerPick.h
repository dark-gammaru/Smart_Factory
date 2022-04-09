// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HoldableObjectEnum.h"
#include "PlayerPick.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_API UPlayerPick : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerPick();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Pick(FHitResult);
	bool IsPickable(FHitResult);

	// Used for Incubater check
	bool IsInteractable(bool, EHabitat);

	AActor* UseRightHandObject();


private:
	APawn* PlayerCharacterRef;

	int32 CurrentWeight;

	const int32 MaxWeight = 5;

	int32 HoldingObjectCount = 0;

	TArray<USceneComponent *> HoldPositions;

	TArray<AActor*> HoldingObjectArray;

	AActor* RightHandObject;

	void HoldRightHand();

#pragma region Swap
public:
	void ResetSwapValues();

private:
	void Swap(float MouseAxis);

	// Temporary save incubator's condition for swap.
	bool bIsSwappable;
	bool bSwapIsAnimal;
	EHabitat SwapHabitat;

	int32 SelectedIndex;

	void ReorderObjects();

#pragma endregion

};
