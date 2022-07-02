// Fill out your copyright notice in the Description page of Project Settings.


#include "Supply.h"

void USupply::DelayedDestroy() {
	static FTimerHandle TimerHandle;
	auto Owner = GetOwner();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([Owner]()
		{
			Owner->Destroy();
		}), 1.f, false, 2.f);
}
