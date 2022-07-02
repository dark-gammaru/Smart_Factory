
#pragma once

#include "CoreMinimal.h"
#include "Commodity.h"
#include "Components/ActorComponent.h"
#include "DataTableRow.h"
#include "Engine/DataTable.h"
#include "HoldableObject.h"
#include "HoldableObjectEnum.h"
#include "Misc/OutputDeviceNull.h"
#include "ChaosDestructable.h"
#include "Incubator.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_API UIncubator : public UChaosDestructable
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:	
	void PutCommodity(AActor*);

	void PutSupply(AActor*);
	
	UFUNCTION(BlueprintCallable)
	bool IsAnimal();

	EHabitat GetHabitat();

	UFUNCTION(BlueprintCallable)
	void SetHabitat(EHabitat Habitat);

	UFUNCTION(BlueprintCallable)
	void CheckTime(FDateTime CurrentTime);

	UFUNCTION(BlueprintCallable)
	void Manufacture();

	UFUNCTION(BlueprintCallable)
	void AbortCommodity();

	bool IsInteractable(UCommodity*);
	
	void MakeDestructable() override;

	UPROPERTY(BlueprintReadOnly)
	float Progress = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float WaterLeft = 1.f;

	UPROPERTY(BlueprintReadOnly)
	float FoodLeft = 1.f;


private:
	UPROPERTY(EditAnywhere)
	EHabitat IncubatorHabitat;

	UPROPERTY(EditAnywhere)
	bool bIsAnimal;

	UPROPERTY()
	UCommodity* GrowingCommodityRef;

	int32 CommodityGrowthDuration;

	FProductRow* ResultRow;

	UPROPERTY()
	FDateTime StartGrowingTime;

	void CalculateProgress(FDateTime CurrentTime);

	void SetPosition(AActor*);

	UPROPERTY()
	FTimespan PassedTime = 0;

	UPROPERTY()
	FDateTime LastTime = 0;

	void ResetVariables();
};
