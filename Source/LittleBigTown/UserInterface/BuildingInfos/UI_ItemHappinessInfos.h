// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
//TEXT
#include "LittleBigTown/UserInterface/BuildingInfos/UI_HappinessInfos.h"

#include "UI_ItemHappinessInfos.generated.h"

UCLASS()
class LITTLEBIGTOWN_API UUI_ItemHappinessInfos : public UUserWidget
{
	GENERATED_BODY()

public :

		void NativePreConstruct() override;

		void SetSpecialtyName(const FText& Name);
		
		void SetInformations(int32 Count, float Satisfaction);

		const int32 GetCount() const;

protected : 

	UPROPERTY(Meta = (BindWidget))
		class UTextBlock* TB_SpecialtyName;

	UPROPERTY(Meta = (BindWidget))
		class UTextBlock* TB_SpecialtyCount;

	UPROPERTY(Meta = (BindWidget))
		class UProgressBar* PB_Satisfaction;

	UPROPERTY(EditAnywhere)
		bool Resize { false };

};
