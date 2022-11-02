// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/SlateEnums.h"

#include "UI_ComboBoxBuildSelection.generated.h"


UCLASS(Abstract)
class LITTLEBIGTOWN_API UUI_ComboBoxBuildSelection : public UUserWidget
{
	GENERATED_BODY()

public:

	void NativeConstruct() override;

	// Update the Child Widget Switcher from ComboBox Index
	void ComboBoxInteraction(int32 SwitcherIndex);

	// Reset ComboBox Option selection
	// Call ResetChildElement at first Index
	void ResetComboBox();

	// Reset BuildingSelection Child Element of WidgetSwitcher
	// Call Reset Interface (BuildingSelection)
	void ResetChildElementAt(int index);

	// Called by delegate OnSelectionChanged (ComboBox)
	// Call ComboBoxInteraction
	// Proceeds a check to ensure Index from ComboBox is not out of range for WidgetSwitcher
	UFUNCTION()
	void UpdateFromNewSelection(FString SelectedItem, ESelectInfo::Type SelectionType);

protected:

	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
		class UComboBoxString* ComboBox;
	
		
	

};
