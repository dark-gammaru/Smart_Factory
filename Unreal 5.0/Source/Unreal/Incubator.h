
#pragma once

#include "CoreMinimal.h"
#include "Commodity.h"
#include "Components/ActorComponent.h"
#include "DataTableRows.h"
#include "Engine/DataTable.h"
#include "HoldableObject.h"
#include "HoldableObjectEnum.h"
#include "Incubator.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_API UIncubator : public UActorComponent
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

	bool IsEmpty();

	void OpenUI();

	UFUNCTION(BlueprintCallable)
	void CheckTime(FDateTime CurrentTime);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UDataTable* CommodityTable;

private:
	UPROPERTY(EditAnywhere)
	EHabitat IncubatorHabitat;

	UPROPERTY(EditAnywhere)
	bool bIsAnimal;

	UCommodity* GrowingCommodityRef;
	int32 CommodityGrowthDuration;

	FCommodityRow* ResultRow;

	FDateTime StartGrowingTime;

	float CalculateProgress(FDateTime CurrentTime);

};
