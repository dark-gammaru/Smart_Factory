#include "DonationManager.h"
#include "SmartFactoryGameInstance.h"

float UDonationManager::CumulativeDistribution() {
	if (CurrentDonation == 0) {
		return 0.f;
	}
	else if (CurrentDonation == MaxDonation[CurrentPhase]) {
		return 1.f;
	}

	float Ratio = CurrentDonation / MaxDonation[CurrentPhase];
	return 0.5 * (1 + erf((Ratio - Mean) / (sqrt(2 * Variance))));
}


float UDonationManager::Donate(int32 DonationAmount) {
	Cast<USmartFactoryGameInstance>(GetWorld()->GetGameInstance())->CurrencyChange(-DonationAmount);

	CurrentDonation += DonationAmount;

	SavedDistribution = CumulativeDistribution();
	UE_LOG(LogTemp, Warning, TEXT("CDF %f"), SavedDistribution);
	return SavedDistribution;
}

float UDonationManager::GetMaxDonation() {
	return MaxDonation[CurrentPhase];
}

float UDonationManager::GetCurrentMaxDonation() {
	return MaxDonation[CurrentPhase] - CurrentDonation;
}

float UDonationManager::GetDistribution() {
	return SavedDistribution;
}

void UDonationManager::ResetDistribution() {
	CurrentDonation = 0;
	SavedDistribution = 0;
}