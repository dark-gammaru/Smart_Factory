// Fill out your copyright notice in the Description page of Project Settings.


#include "WallManager.h"
#include "TimerManager.h" 
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UWallManager::UWallManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UWallManager::CheckDonation(bool testWin) {
	//Character Watch Wall
	//DEBUG
	if (testWin) {
		DonationWin();
	}
	else {
		DonationLose();
	}
}

void UWallManager::Donate(int32 Amount, int32 MaxAmount) {

}

void UWallManager::DonationWin() {
	UE_LOG(LogTemp, Warning, TEXT("Win"));
	if (CurrentLife == 4) {
		//Kill and Next Phase
		++CurrentPhase;
		CurrentLife = 2;
	}
	else {
		//Next
		MoveWallVertical(CurrentLife + 1, CurrentLife);
		++CurrentLife;
	}
}


void UWallManager::DonationLose() {
	UE_LOG(LogTemp, Warning, TEXT("Lose"));
	if (CurrentLife == 0) {
		//Die
	}
	else {
		MoveWallVertical(CurrentLife - 1, CurrentLife);
		--CurrentLife;
	}
}


/// <summary>
/// Move wall holders and wall parts vertically. [LSH]
/// </summary>
/// <param name="TargetWallArray">Target wall holder of current phase.</param>
/// <param name="DownTargetIndex">Index of wall holder that goes down.</param>
/// <param name="UpTargetIndex">Index of wall holder that goes up.</param>
void UWallManager::MoveWallVertical(int32 DownTargetIndex, int32 UpTargetIndex) {
	TArray<AActor*> TargetWallArray;
	WallPhaseHolders[CurrentPhase]->GetAttachedActors(TargetWallArray, true);

	AActor* UpHolder = nullptr;
	AActor* DownHolder = nullptr;
	for (AActor* holder : TargetWallArray) {
		if (UKismetSystemLibrary::GetDisplayName(holder)[7] - '1' == DownTargetIndex) {
			DownHolder = holder;
		}
		else if (UKismetSystemLibrary::GetDisplayName(holder)[7] - '1' == UpTargetIndex) {
			UpHolder = holder;
		}
	}

	if (UpHolder == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("UPNULL"));
		return;
	}
	if (DownHolder == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("DOWNNULL"));
		return;
	}

	// Get wall parts and apply impulse to them.
	TArray<AActor*> DownWalls;
	DownHolder->GetAttachedActors(DownWalls);
	for(AActor* Wall : DownWalls)
	{
		Wall->FindComponentByClass<UStaticMeshComponent>()->SetSimulatePhysics(true);
		Wall->FindComponentByClass<UStaticMeshComponent>()->SetEnableGravity(true);
		Wall->FindComponentByClass<UStaticMeshComponent>()->AddImpulse(FVector(0, 0, -1000.f), "", true);
	}
	// Reset Physics and Gravity and attatch walls to holder again.
	GetWorld()->GetTimerManager().SetTimer(UpWaitHandle, FTimerDelegate::CreateLambda([=]() 
		{
			for (AActor* Wall : DownWalls)
			{
				Wall->FindComponentByClass<UStaticMeshComponent>()->SetSimulatePhysics(false);
				Wall->FindComponentByClass<UStaticMeshComponent>()->SetEnableGravity(false);
				Wall->AttachToActor(DownHolder, FAttachmentTransformRules::KeepWorldTransform);
			}
		}), 2.f, false);

	
	// Move wall smoothly with lambda function.
	FVector OriginalLocation;
	FVector MovedLocation;

	OriginalLocation = UpHolder->GetActorLocation();
	MovedLocation = OriginalLocation + FVector(0, 0, 350.f);
	float WaitTime = 0.01f;  // Call lambda function every 0.01 second.
	GetWorld()->GetTimerManager().SetTimer(DownWaitHandle, FTimerDelegate::CreateLambda([=]()
		{
			alpha = FMath::Clamp(alpha + 0.005f, 0.f, 1.f);
			if (UpHolder) {
				UpHolder->SetActorLocation(FMath::Lerp(OriginalLocation, MovedLocation, alpha));
				if (alpha >= 1.f) {
					GetWorld()->GetTimerManager().ClearTimer(DownWaitHandle);
					alpha = 0.f;
				}
			}
		}), WaitTime, true, 2.f);  // Wait 2 seconds
}
