// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChaosDestructionTriggerVolume.h"
#include "Components/ActorComponent.h"
#include "WallManager.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_API UWallManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWallManager();

	UPROPERTY(EditAnywhere)
	TArray<AActor*> WallPhaseHolders;

	UFUNCTION(BlueprintCallable)
	void CheckDonation();

	UPROPERTY(EditAnywhere)
	AChaosDestructionTriggerVolume* ChaosVolume = nullptr;

private:
	void DonationWin();
	void DonationLose();

	void MoveWallVertical(int32 DownTargetIndex, int32 UpTargetIndex);
	void MoveWallHorizontal(int32 Phase, int32 Life);

	int32 CurrentPhase = 0;
	int32 CurrentLife = 2;

	const float FloorHeight = 200.f;
	const float CeilingHeight = 550.f;

	TArray<float> HorizontalWinLocation{ 1355.0f, 1655.0f, 2900.0f };
	TArray<float> HorizontalLoseLocation{ 60.f, 100.f, 180.f };

	int32 DayCount = 0;
	const int32 DonationDay = 1;

	void CallMorningDelegate();
};
