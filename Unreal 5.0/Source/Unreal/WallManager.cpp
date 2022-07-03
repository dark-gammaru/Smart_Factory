// Fill out your copyright notice in the Description page of Project Settings.


#include "WallManager.h"
#include "TimerManager.h"
#include "DonationManager.h"
#include "SmartFactoryGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UWallManager::UWallManager()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UWallManager::CheckDonation() {
	DonationWin();
	return;
	if (DayCount == DonationDay) {
		float rnd = FMath::FRand();
		if (rnd <= GetOwner()->FindComponentByClass<UDonationManager>()->GetDistribution()) {
			DonationWin();
		}
		else {
			DonationLose();
		}
		GetOwner()->FindComponentByClass<UDonationManager>()->ResetDistribution();
		DayCount = 0;
	}
	else {
		DayCount++;
		CallMorningDelegate();
	}
}

void UWallManager::DonationWin() {
	UE_LOG(LogTemp, Warning, TEXT("Win"));
	if (CurrentLife == 4) {
		//Kill and Next Phase
		MoveWallHorizontal(CurrentPhase, CurrentLife);
		++CurrentPhase;
		GetOwner()->FindComponentByClass<UDonationManager>()->CurrentPhase++;
		ChaosVolume->ChangeSize(CurrentPhase);
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
	if (CurrentLife <= 0) {
		//Die
		MoveWallHorizontal(CurrentPhase, CurrentLife);
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
	static FTimerHandle UpWaitHandle;
	static FTimerHandle DownWaitHandle;

	TArray<AActor*> TargetWallArray;
	WallPhaseHolders[CurrentPhase]->GetAttachedActors(TargetWallArray, true);

	AActor* UpHolder = nullptr;
	AActor* DownHolder = nullptr;
	for (AActor* holder : TargetWallArray) {
		if (holder->ActorHasTag(FName(FString::Printf(TEXT("WALL%d"), DownTargetIndex)))) {
			DownHolder = holder;
		}
		else if (holder->ActorHasTag(FName(FString::Printf(TEXT("WALL%d"), UpTargetIndex)))) {
			UpHolder = holder;
		}

	}

	if (UpHolder == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("UPNULL"));
		return;
	}
	if (DownHolder == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("DOWNNULL"));
		return;
	}

	// Move wall smoothly with lambda function.
	// Move wall down
	FVector OriginalLocation = DownHolder->GetActorLocation();
	FVector MovedLocation = OriginalLocation + FVector(0, 0, -350.f);
	ChaosVolume->SetActorLocation(MovedLocation);

	float CallDelay = 0.01f;  // Call lambda function every 0.01 second.
	static float downAlpha = 0.f;
	GetWorld()->GetTimerManager().SetTimer(DownWaitHandle, FTimerDelegate::CreateLambda([=]()
		{
			if (downAlpha <= 0.f) { // To avoid bug, delay MakeDestroyable.
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), VerticalDownCue, OriginalLocation, FRotator::ZeroRotator, 1.f, 1.f, 0.f, BasicAttenuation);
				ChaosVolume->MakeDestructable();
				downAlpha = 0.1f;
			}
			else if (downAlpha >= 1.f) { // Exit lambda.
				GetWorld()->GetTimerManager().ClearTimer(DownWaitHandle);
				downAlpha = 0.f;
			}
			else {
				downAlpha = FMath::Clamp(downAlpha + 0.05f * downAlpha + 0.05f, 0.f, 1.f); // Change location very fast.
				DownHolder->SetActorLocation(FMath::Lerp(OriginalLocation, MovedLocation, downAlpha));
			}
		}), CallDelay, true, 1.f);

	// Move wall up
	OriginalLocation = UpHolder->GetActorLocation();
	MovedLocation = OriginalLocation + FVector(0, 0, 350.f);
	static float upAlpha = 0.f;
	GetWorld()->GetTimerManager().SetTimer(UpWaitHandle, FTimerDelegate::CreateLambda([=]()
		{
			if (upAlpha <= 0.f) {
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), VerticalUpCue, OriginalLocation, FRotator::ZeroRotator, 1.f, 1.f, 0.f, BasicAttenuation);
			}
			upAlpha = FMath::Clamp(upAlpha + 0.005f, 0.f, 1.f);
			UpHolder->SetActorLocation(FMath::Lerp(OriginalLocation, MovedLocation, upAlpha));
			if (upAlpha >= 1.f) {
				GetWorld()->GetTimerManager().ClearTimer(UpWaitHandle);
				upAlpha = 0.f;
				UE_LOG(LogTemp, Warning, TEXT("MoveEnded"));
				CallMorningDelegate();
			}
		}), CallDelay, true, 3.f);  // Wait 3 seconds
}

/// <summary>
/// Move wall holders and wall parts horizontally. [LSH]
/// </summary>
/// <param name="Phase"> "Current" Phase before it changes.</param>
/// <param name="Life"> "Current" Life before it changes. </param>
void UWallManager::MoveWallHorizontal(int32 Phase, int32 Life) {
	static float alpha = 0.01f;
	static FTimerHandle HorizontalWaitHandle;
	bool isWin = Life == 4;

	// Get move target wall holder.
	TArray<AActor*> TargetWallArray;
	WallPhaseHolders[Phase]->GetAttachedActors(TargetWallArray, true);

	AActor* TargetHolder = nullptr;
	for (AActor* holder : TargetWallArray) {
		if (holder->ActorHasTag(FName(FString::Printf(TEXT("WALL%d"), Life)))) {
			TargetHolder = holder;
			break;
		}
	}

	// Move wall smoothly with lambda function.
	FVector OriginalLocation;
	FVector MovedLocation;

	if (TargetHolder != nullptr) {
		OriginalLocation = TargetHolder->GetActorLocation();
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Horizontal move target holder error"));
		return;
	}
	if (Phase % 2 == 0) {
		if (isWin) { // Win, X Axis
			MovedLocation = FVector(HorizontalWinLocation[Phase], OriginalLocation.Y, OriginalLocation.Z);
		}
		else { // Lose, X Axis
			MovedLocation = FVector(HorizontalLoseLocation[Phase], OriginalLocation.Y, OriginalLocation.Z);
		}
	}
	else {
		if (isWin) { // Win, Y Axis
			MovedLocation = FVector(OriginalLocation.X, HorizontalWinLocation[Phase], OriginalLocation.Z);
		}
		else { // Lose, Y Axis
			MovedLocation = FVector(OriginalLocation.X, HorizontalLoseLocation[Phase], OriginalLocation.Z);
		}
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HorizontalCue, OriginalLocation, FRotator::ZeroRotator, 1.f, 1.f, 0.f, BasicAttenuation);

	if (isWin) {
		alpha = 0.01f;
		GetWorld()->GetTimerManager().SetTimer(HorizontalWaitHandle, FTimerDelegate::CreateLambda([=]() {
			if (alpha <= 1.0f) {
				alpha = FMath::Clamp(alpha + 0.02f * alpha, 0.f, 1.f);
				if (TargetHolder) {
					TargetHolder->SetActorLocation(FMath::Lerp(OriginalLocation, MovedLocation, alpha));
					if (alpha == 1.f) { // This will cause CPU's death.
						// TODO : CPU death Effect.
						UGameplayStatics::PlaySoundAtLocation(GetWorld(), HumanCue, OriginalLocation, FRotator::ZeroRotator, 1.f, 1.f, 0.f, BasicAttenuation);
						CallMorningDelegate();
						alpha = 1.5f;
					}
				}
				else {
					GetWorld()->GetTimerManager().ClearTimer(HorizontalWaitHandle);
					UE_LOG(LogTemp, Error, TEXT("Horizontal Wall error : No holder"));
					CallMorningDelegate();
				}
			}
			else {
				alpha = alpha + 0.01f;
				if (alpha >= 4.f && alpha < 5.f && TargetHolder) {
					TargetHolder->SetActorLocation(FMath::Lerp(MovedLocation, MovedLocation + FVector(0, 0, 300.f), alpha - 4.f));
				}
				else if (alpha > 5.f) {
					if (TargetHolder) {
						GetWorld()->GetTimerManager().ClearTimer(HorizontalWaitHandle);
						alpha = 0.f;
						// After winning, destroy walls for next phase
						TArray<AActor*> ChildWallArray;
						TargetHolder->GetAttachedActors(ChildWallArray, true);
						for (auto Wall : ChildWallArray) {
							Wall->Destroy();
						}
						TargetHolder->Destroy();
					}
				}
			}
			}), 0.02f, true);
	}
	else {
		alpha = 0.01f;
		GetWorld()->GetTimerManager().SetTimer(HorizontalWaitHandle, FTimerDelegate::CreateLambda([=]() {
			alpha = FMath::Clamp(alpha + 0.02f * alpha, 0.f, 1.f);
			if (TargetHolder) {
				TargetHolder->SetActorLocation(FMath::Lerp(OriginalLocation, MovedLocation, alpha));
				ChaosVolume->SetActorLocation(FMath::Lerp(OriginalLocation, MovedLocation, FMath::Clamp(alpha + 0.05f, 0.f, 1.f)));
				ChaosVolume->MakeDestructable();
				if (alpha >= 0.7f) { // This will cause player's death.
					GetWorld()->GetTimerManager().ClearTimer(HorizontalWaitHandle);
					alpha = 0.f;
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), HumanCue, OriginalLocation, FRotator::ZeroRotator, 1.f, 1.f, 0.f, BasicAttenuation);
					Cast<USmartFactoryGameInstance>(GetWorld()->GetGameInstance())->GameOverDelegate.Broadcast();
				}
			}
			else {
				GetWorld()->GetTimerManager().ClearTimer(HorizontalWaitHandle);
				UE_LOG(LogTemp, Error, TEXT("Horizontal Wall error : No holder"));
				CallMorningDelegate();
			}
			}), 0.02f, true);
	}
}

void UWallManager::CallMorningDelegate() {
	Cast<USmartFactoryGameInstance>(GetWorld()->GetGameInstance())->MorningDelegate.Broadcast();
}