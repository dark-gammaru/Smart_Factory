// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "WallManager.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_API UWallManager : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWallManager();

	UPROPERTY(EditAnywhere)
	TArray<AActor*> WallPhaseHolders;

	UFUNCTION(BlueprintCallable)
	void CheckDonation(bool testWin);

	UFUNCTION(BlueprintCallable)
	void Donate(int32 Amount, int32 MaxAmount);

private:
	void DonationWin();
	void DonationLose();

	void MoveWallVertical(int32 DownTargetIndex, int32 UpTargetIndex);

	int32 CurrentPhase = 0;
	int32 CurrentLife = 2;

	const float FloorHeight = 200.f;
	const float CeilingHeight = 550.f;

	float alpha = 0.f;
	FTimerHandle UpWaitHandle;
	FTimerHandle DownWaitHandle;
};
