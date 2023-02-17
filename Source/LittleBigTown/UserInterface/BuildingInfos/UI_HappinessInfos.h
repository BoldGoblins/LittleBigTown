// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LittleBigTown/UserInterface/Templates/Layers/BGItemSelectionWidget.h"

#include "LittleBigTown/Core/Resident.h"

#include "UI_HappinessInfos.generated.h"


UCLASS()
class LITTLEBIGTOWN_API UUI_HappinessInfos : public UBGItemSelectionWidget
{
	GENERATED_BODY()

public :

		void NativeConstruct() override;

		void SetInformations(class ABuilding* Building, bool bNewDisplay = true) override;

		void Reset() override;

		void OnItemClicked(class UBGButton* Button) override;

private :

	void SetClassesInformations(class AResidentialBuilding* ResBuilding, bool bNewDisplay);

	void SetSubClassesInformations(class UBGButton* Button, class AResidentialBuilding* ResBuilding, bool NewDisplay);

	class UUI_ItemHappinessInfos* GetSpeItem(const TEnumAsByte<enum ECitySpecialty>& Specialty);

	bool IsSubClassDisplayed() const;

	void ResetSubClassesWidget();

	class AMainGameState* MainGameState { nullptr };

	class AMainPlayerController* MainPlayerController { nullptr };

	TArray <class UUI_ItemHappinessInfos*> ArrItems{};
	 
protected : 

	UPROPERTY(Meta = (BindWidget))
		class UVerticalBox* VB_Main;

	UPROPERTY(Meta = (BindWidget))
		class UVerticalBox* SubClassWidget;

};
