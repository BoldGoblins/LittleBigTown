// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIBuildingButton.h"
#include "Components/VerticalBox.h"
#include "MainPlayerController.h"
#include "UI_BuildingSelection.h"
#include "MainGameMode.h"
#include "Types/SlateEnums.h"
#include "LittleBigTown.h"

#include "ThematicUI_Template.generated.h"

/*
UCLASS(Abstract)
class LITTLEBIGTOWN_API UThematicUI_Template : public UUserWidget
{
	GENERATED_BODY()

public : 

	virtual void NativeConstruct() override;

	// Update Thematic Widget with Map of Themes submitted by a Constructible Slot
	// Check if Map in parameters as same Num() as Child Buttons of the VerticalBox
	// Called in BPPlayerController (in Editor)
	UFUNCTION(BlueprintCallable)
		void UpdateAndDisplayInterface(TEnumAsByte <ESlotSize> SlotSize);

protected :

	UPROPERTY(meta = (BindWidget))
		UWidgetSwitcher* WidgetSwitcher;


};
*/

#define BUILDING_SELECTION_WIDGET_POS 2

UCLASS(Abstract)
class LITTLEBIGTOWN_API UThematicUI_Template : public UUserWidget
{
	GENERATED_BODY()

public :

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
		void UpdateAndDisplayInterface(TEnumAsByte <ESlotSize> SlotSize, TEnumAsByte <ESlotType> SlotType);
	/*
	UFUNCTION(BlueprintCallable)
		FString GetComboBoxSelectedOption() { return ComboBox->GetSelectedOption(); }
		*/
	UFUNCTION(BlueprintCallable)
		void UpdateFromNewSelection(FString String, ESelectInfo::Type Type);

	UFUNCTION(BlueprintCallable)
		void HideWidget() { SetVisibility(ESlateVisibility::Collapsed); }

	UFUNCTION(BlueprintCallable)
		UUI_BuildingSelection* GetBuildingSelectionWidget() { return BuildingSelectionWidget; }

	TArray <FString> CheckTypeAndSize(TEnumAsByte <ESlotSize> SlotSize, TEnumAsByte <ESlotType> SlotType);

protected : 

	UPROPERTY(meta = (BindWidget))
		UVerticalBox* VerticalBox;

	UPROPERTY(meta = (BindWidget))
		class UComboBoxString* ComboBox;

	UPROPERTY(BlueprintReadOnly)
		UUI_BuildingSelection* BuildingSelectionWidget;

	AMainPlayerController* PlayerController;

	AMainGameMode* GameMode;

	UPROPERTY(BlueprintReadOnly)
		TEnumAsByte <ESlotType> LastSlotType;

	UPROPERTY(BlueprintReadOnly)
		TEnumAsByte <ESlotSize> LastSlotSize;
};
