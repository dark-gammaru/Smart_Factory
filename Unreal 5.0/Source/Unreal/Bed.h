#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChaosDestructable.h"
#include "Bed.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_API UBed : public UChaosDestructable
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isClickedOnce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int clickCount;		
};
