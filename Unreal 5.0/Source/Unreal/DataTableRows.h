// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "Engine/StaticMesh.h"	
#include "Engine/BlueprintGeneratedClass.h"
#include "CoreMinimal.h"
#include "DataTableRows.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FCommodityRow : public FTableRowBase
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

class UNREAL_API DataTableRows
{
public:
	DataTableRows();
	~DataTableRows();
};
