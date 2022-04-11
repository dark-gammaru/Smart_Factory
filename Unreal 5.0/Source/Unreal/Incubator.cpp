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

/// <summary>
/// Test Function to check PlayerInteraction is working properly. [LSH]
/// </summary>
/// <param name=""></param>
void UIncubator::OpenUI(void) {
	UE_LOG(LogTemp, Warning, TEXT("Open Incubator Successed"));
}

/// <summary>
/// Start growing commodity. Set parent, position of commodity and save growing time. [LSH]
/// </summary>
/// <param name="CommodityRef"> : Reference of Commodity</param>
void UIncubator::PutCommodity(AActor* CommodityRef) {
	GrowingCommodityRef = CommodityRef->FindComponentByClass<UCommodity>();
	
	SetPosition(CommodityRef);

	CommodityGrowthDuration = GrowingCommodityRef->GetGrowthTime();
	StartGrowingTime = Cast<USmartFactoryGameInstance>(GetWorld()->GetGameInstance())->GetGameTime();
	ResultRow = CommodityTable->FindRow<FCommodityRow>(*GrowingCommodityRef->GetName(), "");

	if (ResultRow == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Table Error"));
		return;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Success"));
	}
}


void UIncubator::Manufacture() {
	GrowingCommodityRef->GetOwner()->Destroy();
	GrowingCommodityRef = nullptr;
	AActor* Product = Cast<AActor>(GetWorld()->SpawnActor(ResultRow->Product));
	Product->DisableComponentsSimulatePhysics();
	SetPosition(Product);
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
/// Check if incubator is empty. [LSH]
/// </summary>
/// <returns>True if incubator is empty.</returns>
bool UIncubator::IsEmpty() {
	return GrowingCommodityRef == nullptr;
}

/// <summary>
/// Check time and change progress. Registered to CheckTimeDelegate, called every 5 seconds. [LSH]
/// </summary>
/// <param name="CurrentTime"> : Current time from GameInstance.</param>
void UIncubator::CheckTime(FDateTime CurrentTime) {
	if (GrowingCommodityRef) {
		if (CalculateProgress(CurrentTime) >= 10.f) {
			Manufacture();
		}
		else if (CalculateProgress(CurrentTime) >= 100.f) {
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
/// Set relative position of actor to center of incubator.
/// </summary>
/// <param name="TargetActor">Actor to move position.</param>
void UIncubator::SetPosition(AActor* TargetActor) {
	TargetActor->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepRelativeTransform);
	if (!bIsAnimal) {
		TargetActor->SetActorRelativeLocation(FVector(0.f, 0.f, 160.f));
	}
	else {
		TargetActor->SetActorRelativeLocation(FVector(0.f, 0.f, 200.f));
	}
	TargetActor->SetActorRelativeRotation(FQuat::Identity);
}