

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HoldableObject.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_API UHoldableObject : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHoldableObject();

public:	
	int32 GetWeight(void);

	UFUNCTION(BlueprintCallable)
	FString GetName();

	UFUNCTION(BlueprintCallable)
	int32 GetPrice();

	UFUNCTION(BlueprintCallable)
	bool IsHoldable();

	UFUNCTION(BlueprintCallable)
	void MakeUnholdable();

protected:
	UPROPERTY(EditAnywhere)
	FString Name;

	UPROPERTY(EditAnywhere)
	int32 Price;

	UPROPERTY(EditAnywhere)
	int32 Weight;

	bool bIsHoldable = true;
};
