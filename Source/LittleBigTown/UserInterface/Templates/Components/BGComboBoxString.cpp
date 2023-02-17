// Fill out your copyright notice in the Description page of Project Settings.


#include "LittleBigTown/UserInterface/Templates/Components/BGComboBoxString.h"
#include "Components/ComboBoxString.h"

void UBGComboBoxString::NativeConstruct()
{
	Super::NativeConstruct();

	ComboBox->OnSelectionChanged.AddDynamic(this, &ThisClass::BroadcastSelectionChangedDelegate);
}

void UBGComboBoxString::BroadcastSelectionChangedDelegate(FString Option, ESelectInfo::Type SelectionType)
{
	if (bCanBroadcastSelectionChanged)
		OnBGComboBoxStringChanged.Broadcast(this, Option, ComboBox->GetSelectedIndex());
}

void UBGComboBoxString::Populate(const TArray <FString>& Options, bool bDefaultOption)
{
	bCanBroadcastSelectionChanged = false;

	if (ComboBox->GetOptionCount() != 0)
		ComboBox->ClearOptions();

	if (bDefaultOption)
		ComboBox->AddOption(DefaultOption.ToString());

	for (const auto& Option : Options)
		ComboBox->AddOption(Option);

	ComboBox->RefreshOptions();
	ComboBox->SetSelectedIndex(0);
		
	bCanBroadcastSelectionChanged = true;
}
