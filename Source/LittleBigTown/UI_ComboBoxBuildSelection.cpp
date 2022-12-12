// Fill out your copyright notice in the Description page of Project Settings.
#include "UI_ComboBoxBuildSelection.h"

#include "Components/WidgetSwitcher.h"
#include "Components/ComboBoxString.h"
#include "LittleBigTown.h"
#include "UI_BuildingSelection.h"


void UUI_ComboBoxBuildSelection::NativeConstruct()
{
	Super::NativeConstruct();

	ComboBox->OnSelectionChanged.AddUniqueDynamic(this, &ThisClass::UpdateFromNewSelection);
}

void UUI_ComboBoxBuildSelection::ResetChildElementAt(int index)
{
	auto Element = Cast <UUI_BuildingSelection> (WidgetSwitcher->GetChildAt(index));

	if (Element)
		Element->ResetScrollBox();
}

void UUI_ComboBoxBuildSelection::UpdateFromNewSelection(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (!ComboBox || ( ComboBox->GetOptionCount() != WidgetSwitcher->GetChildrenCount() ) )
		return;

#ifdef DEBUG_ONLY

	checkf(ComboBox->GetOptionCount() == WidgetSwitcher->GetChildrenCount(),
		TEXT("Error in UUI_ComboBoxBuildSelection::UpdateFromNewSelection : ComboBox Options != WidgetSwitcher Children Count."));
	
#endif

	ComboBoxInteraction(ComboBox->GetSelectedIndex());
}

void UUI_ComboBoxBuildSelection::ResetComboBox()
{
	if (!ComboBox)
		return;

	ComboBox->SetSelectedIndex(0);
	ResetChildElementAt(0);

}

void UUI_ComboBoxBuildSelection::ComboBoxInteraction(int32 SwitcherIndex)
{
	if (WidgetSwitcher)
	{
		WidgetSwitcher->SetActiveWidgetIndex(SwitcherIndex);
		ResetChildElementAt(SwitcherIndex);
	}

}

