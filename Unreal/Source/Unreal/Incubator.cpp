// Fill out your copyright notice in the Description page of Project Settings.

#include "SmartFactoryLevelActor.h"
#include "Incubator.h"

// Sets default values for this component's properties
UIncubator::UIncubator()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UIncubator::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UIncubator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

/// <summary>
/// Test Function to check PlayerInteraction is working properly. [LSH]
/// </summary>
/// <param name=""></param>
void UIncubator::OpenUI(void) {
	UE_LOG(LogTemp, Warning, TEXT("Open Incubator Successed"));
}

/// <summary>
/// Start Growing Commodity
/// </summary>
/// <param name="CommodityRef"> : Reference of Commodity</param>
void UIncubator::PutCommodity(AActor* CommodityRef) {
	GrowingCommodityRef = CommodityRef->FindComponentByClass<UCommodity>();
	CommodityRef->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepRelativeTransform);
	if (!bIsAnimal) {
		CommodityRef->SetActorRelativeLocation(FVector(0.f, 0.f, 160.f));
	}
	else {
		CommodityRef->SetActorRelativeLocation(FVector(0.f, 0.f, 130.f));
	}
	CommodityRef->SetActorRelativeRotation(FQuat::Identity);
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


