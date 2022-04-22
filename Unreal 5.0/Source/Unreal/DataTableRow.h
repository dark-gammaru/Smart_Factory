// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "Engine/StaticMesh.h"	
#include "Engine/BlueprintGeneratedClass.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataTableRow.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct FProductRow : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        UBlueprintGeneratedClass* Product;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        UStaticMesh* MiddleModel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        UStaticMesh* FinalModel;
};

USTRUCT(BlueprintType)
struct FHoldableObjectRow : public FTableRowBase 
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UBlueprintGeneratedClass* Object;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* Image;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Price;
};

UCLASS()
class UNREAL_API ADataTableRow : public AActor
{
	GENERATED_BODY()
};
