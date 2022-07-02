

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HoldableObject.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_API UHoldableObject : public UActorComponent
{
	GENERATED_BODY()

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

	UPROPERTY()
	int32 Price;

	UPROPERTY(EditAnywhere)
	int32 Weight = 1;

	bool bIsHoldable = true;
};
