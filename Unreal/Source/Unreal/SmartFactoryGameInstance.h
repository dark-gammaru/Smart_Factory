// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SmartFactoryGameInstance.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCheckTimeDelegate, FDateTime, CurrentTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNightDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMorningDelegate);

UCLASS()
class UNREAL_API USmartFactoryGameInstance : public UGameInstance
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDateTime GameTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Currency;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsNight;

	FDateTime GetGameTime();

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
	FCheckTimeDelegate CheckTimeDelegate;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
	FNightDelegate NightDelegate;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
	FMorningDelegate MorningDelegate;
};
