#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LineTraceFloorStrategy.h"
#include "LineTraceObjectStrategy.h"
#include "PlayerLineTrace.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_API UPlayerLineTrace : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerLineTrace();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	FHitResult GetHitResult();

	bool bIsConstructing = false;

	FRotator GetPlayerRotation();

private:
	FHitResult HitResult;

	LineTraceStrategy* ActiveLineTrace;
	LineTraceStrategy* LineTraceFloor;
	LineTraceStrategy* LineTraceObject;
		
};
