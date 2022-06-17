
#pragma once

#include "CoreMinimal.h"
#include "ChaosDestructable.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREAL_API UChaosDestructable : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFUNCTION(BlueprintCallable)
	virtual void MakeDestructable();

protected:
	UPROPERTY(EditAnywhere)
	UBlueprintGeneratedClass* GeometryCollectionBP;

	FTimerHandle DestructionDelayHandle;

	void Destruction(FVector Location, FRotator Rotation);
};
