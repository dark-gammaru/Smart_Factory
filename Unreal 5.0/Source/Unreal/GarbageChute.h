// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IDeactivator.h"
#include "SmartFactoryGameInstance.h"
#include "PlayerLineTrace.h"
#include "GarbageChute.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_API UGarbageChute : public UActorComponent, public IDeactivator
{
	GENERATED_BODY()

public:	
	UGarbageChute();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Activate(int32);
	void Deactivate();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ObjectPrice = 0;

	void SellObject(AActor*);

	UPROPERTY(EditAnywhere)
	AActor* GarbageChuteBase;

	void DestroyHoldableObject(AActor*);

protected:
	virtual void BeginPlay() override;

private:
	USmartFactoryGameInstance* SmartFactoryGameInstanceRef;
	float LerpAlpha = 0.f;

	float InitialRotationRoll = 0.f;
	float InitialRotationPitch = 0.f;
	float InitialRotationYaw = 0.f;
};
