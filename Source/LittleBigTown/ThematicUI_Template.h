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

#define BUILDING_SELECTION_WIDGET_POS 3
#define COMBOBOX_OPTIONS_NUM 15


UCLASS(Abstract)
class LITTLEBIGTOWN_API UThematicUI_Template : public UUserWidget
{
	GENERATED_BODY()

public :

	virtual void NativeConstruct() override;

	// Called in Editor (BP MainPlayerController)
	// Populate ComboBox, reset BuildingSelectionWidget and populate it (with first index ComboBox Option)
	// Set LastSlotSize/Type for UpdateFromNewSelection Method
	// Set visibility of the whole widget
	UFUNCTION(BlueprintCallable)
		void UpdateAndDisplayInterface(TEnumAsByte<ESlotType> SlotType, TEnumAsByte<ESlotSize> SlotSize);

	// Bind to OnSelectionChanged (ComboBoxString)
	// Reset, Clear and Populate ComboBox according to new option selected
	UFUNCTION(BlueprintCallable)
	void UpdateFromNewSelection(FString String, ESelectInfo::Type Type);

	// BuildingSelectionWidget adress is assigned in Constructor (GetChildAt(BUILDING_SELECTION_WIDGET_POS))
	UFUNCTION(BlueprintCallable)
		UUI_BuildingSelection* GetBuildingSelectionWidget() { return BuildingSelectionWidget; }

	// Check if Slot Type/Size combination are legit, and return the good options associated (to be used to populate a ComboBoxString)
	TArray <FString> CheckTypeAndSize(TEnumAsByte <ESlotSize> SlotSize, TEnumAsByte <ESlotType> SlotType);


protected : 

	UPROPERTY(meta = (BindWidget))
		UVerticalBox* VerticalBox;

	UPROPERTY(meta = (BindWidget))
		class UComboBoxString* ComboBox;

	UPROPERTY(BlueprintReadOnly)
		UUI_BuildingSelection* BuildingSelectionWidget;

	AMainGameMode* GameMode;

	// Used by UpdateFromNewSelection Method
	UPROPERTY(BlueprintReadOnly)
		TEnumAsByte <ESlotType> LastSlotType;

	// Used by UpdateFromNewSelection Method
	UPROPERTY(BlueprintReadOnly)
		TEnumAsByte <ESlotSize> LastSlotSize;

	// Set in Editor (BP_WidgetConstruction)
	// Used in CheckTypeSize (avoid using string litterals and no localizable in code)
	UPROPERTY(EditDefaultsOnly)
		TArray <FText> ComboBoxOptions;


};
