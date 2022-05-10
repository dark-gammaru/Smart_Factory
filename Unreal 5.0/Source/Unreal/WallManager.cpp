// Fill out your copyright notice in the Description page of Project Settings.


#include "WallManager.h"

// Sets default values for this component's properties
UWallManager::UWallManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UWallManager::CheckDonation() {
	//Character Watch Wall
	//DEBUG
	DonationWin();
}

void UWallManager::Donate(int32 Amount, int32 MaxAmount) {

}

void UWallManager::DonationWin() {
	UE_LOG(LogTemp, Warning, TEXT("Win"));
	TArray<AActor*> Walls;
	TArray<AActor*> Walls2;
	WallPhaseHolders[CurrentPhase]->GetAttachedActors(Walls);
	if (CurrentLife == 4) {
		//Kill and Next Phase
		++CurrentPhase;
		CurrentLife = 2;
	}
	else {
//		MoveWallVertical(Walls, CurrentLife + 1, CurrentLife);
		UE_LOG(LogTemp, Warning, TEXT("Name %s"), *Walls[0]->GetName());
		Walls[0]->GetAttachedActors(Walls2, true, true);
		UE_LOG(LogTemp, Warning, TEXT("Inside22 %d"), Walls2.Num());
		++CurrentLife;
	}



	UE_LOG(LogTemp, Warning, TEXT("%d"), Walls2.Num());
}


void UWallManager::DonationLose() {
	UE_LOG(LogTemp, Warning, TEXT("Lose"));
	TArray<AActor*> Walls;
	WallPhaseHolders[CurrentPhase]->GetAttachedActors(Walls);
	if (CurrentLife == 0) {
		//Die
	}
	else {
		//Next
		MoveWallVertical(Walls, CurrentLife - 1, CurrentLife);
		--CurrentLife;
	}

}

void UWallManager::MoveWallVertical(const TArray<AActor*> &TargetWallArray, int32 DownTargetIndex, int32 UpTargetIndex) {
	TArray<AActor*> Walls;
	TargetWallArray[UpTargetIndex]->GetAttachedActors(Walls);
	UE_LOG(LogTemp, Warning, TEXT("Inside %d"), Walls.Num());
	for(AActor * Wall : Walls)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Wall->GetName());
		Wall->FindComponentByClass<UStaticMeshComponent>()->AddForce(FVector(0, 0, 10000.f));
	}
}
