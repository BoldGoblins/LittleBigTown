// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LittleBigTown/UserInterface/Templates/BGMenuSelection.h"

#include "LittleBigTown/Core/Resident.h"

#include "UI_HappinessInfos.generated.h"


UCLASS()
class LITTLEBIGTOWN_API UUI_HappinessInfos : public UBGMenuSelection
{
	GENERATED_BODY()

public :

		void NativePreConstruct() override;

		void NativeConstruct() override;

		UFUNCTION(meta = (DevelopmentOnly))
			void DEBUGPrintClasses();

		UFUNCTION(meta = (DevelopmentOnly))
			void DEBUGPrintSubClasses(const ECitySpecialty& Specialty);

		// Overload pour différents Types dérivant de ABuilding
		void MainDisplay(class AResidentialBuilding* Building, bool NewDisplay);

		void ResetWidget(ESlateVisibility Visible) override;

		void OnItemClicked(class UBGButton* Button) override;

		void SubClassDisplay(class UBGButton* Button, bool NewDisplay);

private :

	TArray <struct FResident>& GetSpeArray(const TEnumAsByte<enum ECitySpecialty>& Specialty);

	class UUI_ItemHappinessInfos* GetSpeItem(const TEnumAsByte<enum ECitySpecialty>& Specialty);

	void EmptySpeArrays();

	void ResetSubClassWidget();

	TArray <struct FResident> m_Industry;

	TArray <struct FResident> m_Finance;

	TArray <struct FResident> m_Science;

	TArray <struct FResident> m_Tourism;

	TArray <struct FResident> m_Crime;

	TArray <struct FResident> m_Military;

	TArray <struct FResident> m_Spiritual;

	class AMainGameState* MainGameState { nullptr };

	TArray <class UUI_ItemHappinessInfos*> ArrItems{};

	TEnumAsByte <EWealthLevels> BuildingWealth { DefaultWealthEnum };
	 
protected : 

	UPROPERTY(Meta = (BindWidget))
		class UVerticalBox* VB_Main;

	UPROPERTY(Meta = (BindWidget))
		class UVerticalBox* SubClassWidget;

};
