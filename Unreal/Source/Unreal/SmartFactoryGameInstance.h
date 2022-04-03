// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SmartFactoryGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_API USmartFactoryGameInstance : public UGameInstance
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDateTime GameTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Currency;
};
