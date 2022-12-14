// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "LBTGameTimeIF.generated.h"

UINTERFACE(MinimalAPI)
class ULBTGameTimeIF : public UInterface
{
	GENERATED_BODY()
};

class LITTLEBIGTOWN_API ILBTGameTimeIF
{
	GENERATED_BODY()

public :

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "TimeManagement")
		void GetDateUpdate(const FDateTime& GameClock);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "TimeManagement")
		void GetTimeUpdate(const FDateTime& GameClock);

};