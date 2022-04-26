// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class UNREAL_API IDeactivator
{
public:
	virtual void Activate(int32) = 0;
	virtual void Deactivate() = 0;

protected:
	bool bIsActivated = false;
};
