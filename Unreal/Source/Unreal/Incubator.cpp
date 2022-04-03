// Fill out your copyright notice in the Description page of Project Settings.


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

bool UIncubator::IsAnimal() {
	return bIsAnimal;
}


EHabitat UIncubator::GetHabitat() {
	return IncubatorHabitat;
}


