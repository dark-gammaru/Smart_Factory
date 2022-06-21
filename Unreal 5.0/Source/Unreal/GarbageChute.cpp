// Fill out your copyright notice in the Description page of Project Settings.

#include "GarbageChute.h"
#include "Kismet/GameplayStatics.h"
#include "HoldableObject.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PrimitiveComponent.h"

UGarbageChute::UGarbageChute()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGarbageChute::BeginPlay() {
	Super::BeginPlay();

	FRotator InitialRotation = GetOwner()->GetActorRotation();
	InitialRotationRoll = InitialRotation.Roll;
	InitialRotationPitch = InitialRotation.Pitch;
	InitialRotationYaw = InitialRotation.Yaw;
	
	SmartFactoryGameInstanceRef = Cast<USmartFactoryGameInstance>(GetWorld()->GetGameInstance());
}


void UGarbageChute::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsActivated) {
		LerpAlpha = FMath::Clamp(LerpAlpha + DeltaTime * 2.5f, 0.f, 1.f);
	}
	else {
		LerpAlpha = FMath::Clamp(LerpAlpha - DeltaTime * 2.5f, 0.f, 1.f);
	}

	GetOwner()->SetActorRotation(FRotator(FMath::Lerp(InitialRotationPitch, InitialRotationPitch + 65.f, LerpAlpha)
		, InitialRotationYaw, InitialRotationRoll));
}

void UGarbageChute::Activate(int32 Price) {
	bIsActivated = true;
	ObjectPrice = Price;
}

void UGarbageChute::Deactivate() {
	bIsActivated = false;
	ObjectPrice = 0;
}

// Sell object and destroy it after few seconds. [LSH]
void UGarbageChute::SellObject(AActor* TargetObject) {
	TargetObject->FindComponentByClass<UStaticMeshComponent>()->SetCollisionProfileName(TEXT("BlockAll"));
	TargetObject->AttachToActor(GarbageChuteBase, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	TargetObject->FindComponentByClass<UStaticMeshComponent>()->SetSimulatePhysics(true);
	SmartFactoryGameInstanceRef->CurrencyChange(TargetObject->FindComponentByClass<UHoldableObject>()->GetPrice());

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), MoneyCue, GetOwner()->GetActorLocation(), FRotator::ZeroRotator);

	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &UGarbageChute::DestroyHoldableObject, TargetObject);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 2, false);
}

void UGarbageChute::DestroyHoldableObject(AActor* Target) {
	Target->Destroy();
}

