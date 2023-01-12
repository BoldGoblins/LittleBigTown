// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Demand.generated.h"

UCLASS()
class LITTLEBIGTOWN_API UUI_Demand : public UUserWidget
{
	GENERATED_BODY()
public :

	void UpdateDemand(float PoorDemand, float MiddleDemand, float RichDemand);

protected:

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* PB_PoorNeg;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* PB_MiddleNeg;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* PB_RichNeg;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* PB_PoorPos;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* PB_MiddlePos;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* PB_RichPos;
	
};
