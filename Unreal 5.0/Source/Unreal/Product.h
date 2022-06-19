// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HoldableObject.h"
#include "HoldableObjectEnum.h"
#include "Product.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREAL_API UProduct : public UHoldableObject
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	bool bIsBadQuality;

	UPROPERTY(EditAnywhere)
	EProductCategory ProductCategory;

	UPROPERTY(EditAnywhere)
	EProductColor ProductColor;
};
