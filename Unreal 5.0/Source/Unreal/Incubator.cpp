// Fill out your copyright notice in the Description page of Project Settings.

#include "Incubator.h"
#include "Supply.h"
#include "Components/LightComponent.h"
#include "SmartFactoryGameInstance.h"

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
	LastTime = 0;
	GrowingCommodityRef = CommodityRef->FindComponentByClass<UCommodity>();
	
	// Preventing player hold a commodity that has been already put.
	GrowingCommodityRef->MakeUnholdable();

	// Attach commodity to incubator and reposition it.
	SetPosition(CommodityRef);

	// Call CloseDoor on blueprint
	FOutputDeviceNull device;
	FString DoorEventString = TEXT("PutCommodity");
	GetOwner()->CallFunctionByNameWithArguments(*DoorEventString, device, NULL, true);

	Progress = 0.f;

	CommodityGrowthDuration = GrowingCommodityRef->GetGrowthTime();
	StartGrowingTime = Cast<USmartFactoryGameInstance>(GetWorld()->GetGameInstance())->GetGameTime();

	ResultRow = Cast<USmartFactoryGameInstance>(GetWorld()->GetGameInstance())->StaticMeshTable->FindRow<FProductRow>(*GrowingCommodityRef->GetName(), "");

	if (!ensure(ResultRow != nullptr)) {
		UE_LOG(LogTemp, Warning, TEXT("Table Error"));
	}
}

void UIncubator::PutSupply(AActor* SupplyRef) {
	SupplyRef->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepRelativeTransform);
	if (SupplyRef->FindComponentByClass<USupply>()->bIsWaterSupply) {
		WaterLeft = FMath::Min(2.f, WaterLeft + 1.f);
		SupplyRef->SetActorRelativeLocation(FVector(50.f, -84.f, 73.f));
	}
	else {
		FoodLeft = FMath::Min(2.f, FoodLeft + 1.f);
		SupplyRef->SetActorRelativeLocation(FVector(50.f, -28.f, 73.f));
	}
	SupplyRef->SetActorRelativeRotation(FRotator(-40.f, 0.f, 0.f));
	SupplyRef->SetActorRelativeScale3D(FVector::OneVector);
	SupplyRef->FindComponentByClass<USupply>()->DelayedDestroy();
}

/// <summary>
/// Destroy growing commodity and spawn product. [LSH]
/// </summary>
void UIncubator::Manufacture() {
	GrowingCommodityRef->GetOwner()->Destroy();
	AActor* Product = Cast<AActor>(GetWorld()->SpawnActor(ResultRow->Product));
	Product->DisableComponentsSimulatePhysics();
	SetPosition(Product);
	Product->FindComponentByClass<UStaticMeshComponent>()->SetSimulatePhysics(true);
	Product->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform); 
	ResetVariables();
}

void UIncubator::AbortCommodity() {
	if (GrowingCommodityRef) {
		GrowingCommodityRef->GetOwner()->Destroy();
	}	
	ResetVariables();
}


bool UIncubator::IsAnimal() {
	return bIsAnimal;
}

EHabitat UIncubator::GetHabitat() {
	return IncubatorHabitat;
}

void UIncubator::SetHabitat(EHabitat Habitat) {
	IncubatorHabitat = Habitat;
}

/// <summary>
/// Check time and change progress. Registered to CheckTimeDelegate, called every 5 seconds. [LSH]
/// </summary>
/// <param name="CurrentTime"> : Current time from GameInstance.</param>
void UIncubator::CheckTime(FDateTime CurrentTime) {
	CalculateProgress(CurrentTime);
	if (GrowingCommodityRef) {
		if (Progress >= 1.f) {
			GrowingCommodityRef->GetOwner()->FindComponentByClass<UStaticMeshComponent>()->SetStaticMesh(ResultRow->FinalModel);
		}
		else if (Progress >= 0.5f) {
			GrowingCommodityRef->GetOwner()->FindComponentByClass<UStaticMeshComponent>()->SetStaticMesh(ResultRow->MiddleModel);
		}
		UE_LOG(LogTemp, Warning, TEXT("%s Growing... %5.1f%%"), *GetOwner()->GetName(), Progress * 100);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Empty Incubator %s : Working"), *GetOwner()->GetName());
	}
}

/// <summary>
/// Calculate growing progress as percentage. [LSH]
/// </summary>
/// <param name="CurrentTime"> : Current Time from GameInstance.</param>
void UIncubator::CalculateProgress(FDateTime CurrentTime) {
	if (GrowingCommodityRef == nullptr) {
		return;
	}
	if (LastTime == 0) {
		LastTime = CurrentTime;
	}
	int32 TimeCount = (CurrentTime - LastTime).GetTotalMinutes() / 20;
	while (TimeCount > 0) {
		if (WaterLeft > 0.f && FoodLeft > 0.f) {
			Progress += 20.f / CommodityGrowthDuration;
		}
		WaterLeft = FMath::Max(0.f, WaterLeft - 0.01f);
		FoodLeft = FMath::Max(0.f, FoodLeft - 0.005f);
		TimeCount--;
	}
	LastTime = CurrentTime;
}


/// <summary>
/// Set relative position of actor to center of incubator. [LSH]
/// </summary>
/// <param name="TargetActor">Actor to move position.</param>
void UIncubator::SetPosition(AActor* TargetActor) {
	TargetActor->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepRelativeTransform);
	if (bIsAnimal) {
		TargetActor->SetActorRelativeLocation(FVector(0.f, 55.f, 70.f));
	}
	else {
		TargetActor->SetActorRelativeLocation(FVector(0.f, 55.f, 86.f));
	}
	TargetActor->SetActorRelativeRotation(FQuat::Identity);
	TargetActor->SetActorRelativeScale3D(FVector::OneVector);
}

void UIncubator::MakeDestructable() {
	// If growing something, then destroy it.
	if (GrowingCommodityRef) {
		GrowingCommodityRef->GetOwner()->Destroy();
		GrowingCommodityRef = nullptr;
	}

	// Unregister Time delegate
	Cast<USmartFactoryGameInstance>(GetWorld()->GetGameInstance())->CheckTimeDelegate.Remove(this, FName("CheckTime"));

	Super::MakeDestructable();
}

void UIncubator::ResetVariables() {
	GrowingCommodityRef = nullptr;
	ResultRow = nullptr;
	PassedTime = 0;
}