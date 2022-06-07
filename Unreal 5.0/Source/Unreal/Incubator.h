
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

public:	
	// Sets default values for this component's properties
	UIncubator();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	void PutCommodity(AActor*);
	
	bool IsAnimal();
	EHabitat GetHabitat();

	UFUNCTION(BlueprintCallable)
	void CheckTime(FDateTime CurrentTime);

	UFUNCTION(BlueprintCallable)
	void Manufacture();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UDataTable* ProductTable;

	bool IsInteractable(UCommodity*);

#pragma region Destructable
	void MakeDestructable() override;

#pragma endregion

private:
	UPROPERTY(EditAnywhere)
	EHabitat IncubatorHabitat;

	UPROPERTY(EditAnywhere)
	bool bIsAnimal;

	UCommodity* GrowingCommodityRef;
	int32 CommodityGrowthDuration;

	FProductRow* ResultRow;

	FDateTime StartGrowingTime;

	float CalculateProgress(FDateTime CurrentTime);

	void SetPosition(AActor*);

};
