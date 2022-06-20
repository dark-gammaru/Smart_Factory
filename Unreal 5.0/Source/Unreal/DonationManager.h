#pragma once

#include <cmath>
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DonationManager.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREAL_API UDonationManager : public UActorComponent
{
	GENERATED_BODY()
		
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 CurrentDonation = 0;

	UFUNCTION(BlueprintCallable)
	float Donate(int32 DonationAmount);

	UFUNCTION(BlueprintCallable)
	float GetMaxDonation();

	UFUNCTION(BlueprintCallable)
	float GetCurrentMaxDonation();

	UFUNCTION(BlueprintCallable)
	float GetDistribution();

	int32 CurrentPhase = 0;

	void ResetDistribution();

private:
	const float Mean = 0.5f;
	const float Variance = 0.10f;

	TArray<float> MaxDonation{1000.f, 3000.f, 10000.f};

	float SavedDistribution;

	float CumulativeDistribution();
};
