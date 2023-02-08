// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LittleBigTown/UserInterface/Templates/BGUserWidget.h"
// struct residential infos definition
#include "LittleBigTown/Core/Structs.h"
#include "LittleBigTown/UserInterface/BuildingInfos/UI_HappinessInfos.h"

#include "UI_BuildingInfos.generated.h"

// FText MakeProportionText(int32 a, int32 b);


UCLASS()
class LITTLEBIGTOWN_API UUI_BuildingInfos : public UBGUserWidget
{
	GENERATED_BODY()


public : 

	void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
		void NewDisplayBuildingInfos(class ABuilding* Building);

		void Hide() override;

	UFUNCTION()
		void BuildingInfosUpdated();

	UFUNCTION()
		void GeneralClicked();

	UFUNCTION()
		void HappinessClicked();

	UUI_HappinessInfos* GetHappinessInfos() const { return HappinessInfosWidget;  }


protected :

	TWeakObjectPtr <class AResidentialBuilding> CurrResBuilding;

	class UUI_General_Infos* GeneralInfosWidget;

	UUI_HappinessInfos* HappinessInfosWidget;

	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
		class UButton* B_General;

	UPROPERTY(meta = (BindWidget))
		class UButton* B_Happiness;


	// --------------------------------------		TEXTBLOCKS		--------------------------------------


	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TB_Name;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TB_Owner;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TB_District;

};
