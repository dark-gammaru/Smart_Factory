#pragma once

#include "CoreMinimal.h"
#include "HoldableObject.h"
#include "HoldableObjectEnum.h"
#include "Commodity.generated.h"

UCLASS()
class UNREAL_API UCommodity : public UHoldableObject
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	FString ScientificName;

	UPROPERTY(EditAnywhere)
	FString Information;

	UPROPERTY(EditAnywhere)
	int32 GrowthTime;

	UPROPERTY(EditAnywhere)
	bool bIsAnimal;

	UPROPERTY(EditAnywhere)
	EHabitat Habitat;

	UPROPERTY(EditAnywhere)
	EProductCategory ProductCategory;

	UPROPERTY(EditAnywhere)
	EProductColor ProductColor;

};
