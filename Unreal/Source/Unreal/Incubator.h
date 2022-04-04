
#pragma once

#include "CoreMinimal.h"
#include "Commodity.h"
#include "Components/ActorComponent.h"
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

private:
	UPROPERTY(EditAnywhere)
	EHabitat IncubatorHabitat;

	UCommodity* GrowingCommodityRef;
	float CommodityGrowthDuration;

	FDateTime StartGrowingTime;

	UPROPERTY(EditAnywhere)
	bool bIsAnimal;

	UFUNCTION(BlueprintCallable)
	void CheckTime(FDateTime CurrentTime);

	float CalculateProgress(FDateTime CurrentTime);
};
