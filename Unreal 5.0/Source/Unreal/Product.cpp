// Fill out your copyright notice in the Description page of Project Settings.


#include "Product.h"
#include "SmartFactoryGameInstance.h"
#include "DataTableRow.h"

void UProduct::BeginPlay() {
	Super::BeginPlay();
	auto ResultRow = Cast<USmartFactoryGameInstance>(GetWorld()->GetGameInstance())->MarketProductTable->FindRow<FMarketProductRow>(*GetName(), "");
	if (ResultRow != nullptr) {
		Price = ResultRow->Price;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Product or MarketProductRow error"));
	}
}

