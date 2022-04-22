// Fill out your copyright notice in the Description page of Project Settings.


#include "SmartFactoryGameInstance.h"

void USmartFactoryGameInstance::CurrencyChange(int32 Amount) {
	Currency += Amount;
	CurrencyDelegate.Broadcast(Currency);
}

FDateTime USmartFactoryGameInstance::GetGameTime(void) {
	return GameTime;
}