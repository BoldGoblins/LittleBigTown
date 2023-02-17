// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LittleBigTown/UserInterface/Templates/Layers/BGSComboBoxWidget.h"
#include "LittleBigTown/Core/Enums.h"

#include "UI_NeedsInfos.generated.h"


UCLASS()
class LITTLEBIGTOWN_API UUI_NeedsInfos : public UBGSComboBoxWidget
{
	GENERATED_BODY()

public : 

	void NativeConstruct() override;

	void SetInformations(ABuilding* Building, bool bNewDisplay = true) override;

	void Reset() override;

	void ComboSelectionChanged(UBGComboBoxString* ComboBox, const FString& Option, int32 Index) override;

private : 

	TArray <FName> SubClassesOptions{};

	// To Overload
	void DisplayInformations(const class AResidentialBuilding* Building);

	void PopulateScrollAndCount(const TTuple <struct FNeeds, int32>& Infos);

	TTuple <struct FNeeds, int32> ComputeInfos(const TArray <struct FResident>& Arr);

protected:  

	UPROPERTY(Meta = (BindWidget))
		class UTextBlock* TB_Count;

	UPROPERTY(Meta = (BindWidget))
		class UGridPanel* GP_Classes;

	UPROPERTY(Meta = (BindWidget))
		class UGridPanel* GP_SubClasses;

	UPROPERTY(Meta = (BindWidget))
		class UVerticalBox* VB_ProgressBars;

	class UBGComboBoxString* CB_Classes { nullptr };

	class UBGComboBoxString* CB_SubClasses{ nullptr };

	class AMainGameState* MainGameState { nullptr };

	class AMainPlayerController* MainPlayerController{ nullptr };
	
};
