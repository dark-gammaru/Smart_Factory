

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

	FString GetName();

protected:
	UPROPERTY(EditAnywhere)
	FString Name;

	UPROPERTY(EditAnywhere)
	int32 Price;

	UPROPERTY(EditAnywhere)
	int32 Weight;
};
