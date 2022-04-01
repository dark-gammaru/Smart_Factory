// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerInteraction.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_API UPlayerInteraction : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerInteraction();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(BlueprintReadWrite);
	bool bIsHit = false;
	UPROPERTY(BlueprintReadWrite);
	bool bIsInteractable = false;

private:
	class UPlayerPick* PlayerPickRef;
	FHitResult GetFirstPhysicsBodyInReach() const;
	FVector GetPlayersReach() const;
	FVector GetPlayersWorldPos() const;

	void Interact(void);
	void IsInteractable(void);

	const float HandDistance = 150.f;
};
