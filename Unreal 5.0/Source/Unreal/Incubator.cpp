// Fill out your copyright notice in the Description page of Project Settings.

#include "Incubator.h"
#include "SmartFactoryGameInstance.h"

// Sets default values for this component's properties
UIncubator::UIncubator()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UIncubator::BeginPlay()
{
	Super::BeginPlay();

	//Register CheckTime to CheckTimeDelegate
	Cast<USmartFactoryGameInstance>(GetWorld()->GetGameInstance())->CheckTimeDelegate.AddDynamic(this, &UIncubator::CheckTime);
}

bool UIncubator::IsInteractable(UCommodity* Item) {
	if (Item->IsAnimal() == bIsAnimal && Item->GetHabitat() == IncubatorHabitat && GrowingCommodityRef == nullptr) {
		return true;
	}
	else {
		return false;
	}
}

/// <summary>
/// Start growing commodity. Set parent, position of commodity and save growing time. [LSH]
/// Call CloseDoor on blueprint, 
/// </summary>
/// <param name="CommodityRef"> : Reference of Commodity</param>
void UIncubator::PutCommodity(AActor* CommodityRef) {
	GrowingCommodityRef = CommodityRef->FindComponentByClass<UCommodity>();

	// Save commodity name for blueprint.
	CommodityName = GrowingCommodityRef->GetName();
	
	// Preventing player hold a commodity that has been already put.
	GrowingCommodityRef->MakeUnholdable();

	// Call CloseDoor on blueprint
	FOutputDeviceNull device;
	FString DoorEventString = TEXT("CloseDoor");
	GetOwner()->CallFunctionByNameWithArguments(*DoorEventString, device, NULL, true);

	// Call SendCommodityInfo on blueprint
	char FuncCallBuf[256];
	FString CommodityEventString = TEXT("SendCommodityInfo");
	_snprintf(FuncCallBuf, sizeof(FuncCallBuf), "%s %d", TCHAR_TO_ANSI(*CommodityEventString),
		GrowingCommodityRef->GetGrowthTime());

	GetOwner()->CallFunctionByNameWithArguments(ANSI_TO_TCHAR(FuncCallBuf), device, NULL, true);

	SetPosition(CommodityRef);

	CommodityGrowthDuration = GrowingCommodityRef->GetGrowthTime();
	StartGrowingTime = Cast<USmartFactoryGameInstance>(GetWorld()->GetGameInstance())->GetGameTime();
	ResultRow = ProductTable->FindRow<FProductRow>(*GrowingCommodityRef->GetName(), "");

	if (ResultRow == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Table Error"));
	}
}

/// <summary>
/// Destroy growing commodity and spawn product. [LSH]
/// </summary>
void UIncubator::Manufacture() {
	GrowingCommodityRef->GetOwner()->Destroy();
	GrowingCommodityRef = nullptr;
	AActor* Product = Cast<AActor>(GetWorld()->SpawnActor(ResultRow->Product));
	Product->DisableComponentsSimulatePhysics();
	SetPosition(Product);
	Product->FindComponentByClass<UStaticMeshComponent>()->SetSimulatePhysics(true);
	Product->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	ResultRow = nullptr;
}


bool UIncubator::IsAnimal() {
	return bIsAnimal;
}

EHabitat UIncubator::GetHabitat() {
	return IncubatorHabitat;
}

/// <summary>
/// Check time and change progress. Registered to CheckTimeDelegate, called every 5 seconds. [LSH]
/// </summary>
/// <param name="CurrentTime"> : Current time from GameInstance.</param>
void UIncubator::CheckTime(FDateTime CurrentTime) {
	if (GrowingCommodityRef) {
		if (CalculateProgress(CurrentTime) >= 100.f) {
			GrowingCommodityRef->GetOwner()->FindComponentByClass<UStaticMeshComponent>()->SetStaticMesh(ResultRow->FinalModel);
		}
		else if (CalculateProgress(CurrentTime) >= 50.f) {
			GrowingCommodityRef->GetOwner()->FindComponentByClass<UStaticMeshComponent>()->SetStaticMesh(ResultRow->MiddleModel);
		}
		UE_LOG(LogTemp, Warning, TEXT("%s Growing... %5.1f%%"), *GetOwner()->GetName(), CalculateProgress(CurrentTime));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Empty Incubator %s : Working"), *GetOwner()->GetName());
	}
}

/// <summary>
/// Calculate growing progress as percentage. [LSH]
/// </summary>
/// <param name="CurrentTime"> : Current Time from GameInstance.</param>
/// <returns>Float percentage of growing progress.</returns>
float UIncubator::CalculateProgress(FDateTime CurrentTime) {
	FTimespan Timespan = CurrentTime - StartGrowingTime;
	return (float)(Timespan.GetHours() * 60 + Timespan.GetMinutes()) / CommodityGrowthDuration * 100;
}


/// <summary>
/// Set relative position of actor to center of incubator. [LSH]
/// </summary>
/// <param name="TargetActor">Actor to move position.</param>
void UIncubator::SetPosition(AActor* TargetActor) {
	TargetActor->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepRelativeTransform);
	if (bIsAnimal) {
		TargetActor->SetActorRelativeLocation(FVector(0.f, 55.f, 57.f));
	}
	else {
		TargetActor->SetActorRelativeLocation(FVector(0.f, 55.f, 86.f));
	}
	TargetActor->SetActorRelativeRotation(FQuat::Identity);
}