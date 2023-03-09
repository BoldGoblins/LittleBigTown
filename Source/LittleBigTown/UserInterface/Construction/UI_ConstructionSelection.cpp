// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_ConstructionSelection.h"

#include "LittleBigTown/GameSystem/BGMainPlayerController.h"
#include "LittleBigTown/GameSystem/MainGameMode.h"
#include "UI_ConstructionButton.h"
#include "UI_ConstructionMain.h"
#include "UI_ConstructionValidation.h"
#include "Components/ScrollBox.h"
#include "Kismet/GameplayStatics.h"
// DEBUG_ONLY
#include "LittleBigTown/Core/Debugger.h"


void UUI_ConstructionSelection::NativeConstruct()
{
	Super::NativeConstruct();

	auto PC { Cast <ABGMainPlayerController> (UGameplayStatics::GetPlayerController(GetWorld(), 0)) };

	if (IsValid(PC))
		PlayerController = PC;

#ifdef DEBUG_ONLY

	checkf(IsValid(PlayerController), TEXT("Error in UUI_BuildingSelection::NativeConstruct, PlayerController == nullptr"));

#endif

}

void UUI_ConstructionSelection::ButtonInteraction(UUI_ConstructionButton* Button)
{

#ifdef DEBUG_ONLY

	checkf(Button, TEXT("Error in UUI_BuildingSelection::ButtonInteraction, Button == nullptr"));

#endif

	if (!Button || !PlayerController)
		return;

	bool temp { Button->GetButtonClicked() };

	if (!temp)
	{
		Button->SetButtonClicked(true);
		LastButtonClicked = Button;
	}

	Button->SetButtonClicked(true);

	const auto ConstructionWidget { PlayerController->GetConstructionWidget() };

	if (ConstructionWidget)
	{
		const auto ValidationWidget { ConstructionWidget->GetConstructionValidationWidget() };
		
		if (ValidationWidget)
			ValidationWidget->DisplayValidationWidget(Button->GetButtonText());

	}

}

void UUI_ConstructionSelection::ResetScrollBox(bool ResetScroll)
{
	if (LastButtonClicked)
		LastButtonClicked->SetButtonClicked(false);

	if (ScrollBox && ResetScroll)
		ScrollBox->ScrollToStart();
}

void UUI_ConstructionSelection::PopulateScrollBox(const TMap <FName, FBuildingContainers>& M, const FString& ComboBoxOption)
{
	if (ScrollBox->GetChildrenCount() != MAX_SCROLLBOX_BUTTONS || M.Num() == 0)
		return;

	auto Arr { ScrollBox->GetAllChildren() };

#ifdef DEBUG_ONLY

	checkf(ScrollBox->GetChildrenCount() == MAX_SCROLLBOX_BUTTONS && M.Num() != 0,
		TEXT("Error in UUI_BuildingSelection::PopulateScrollBox : Map is empty or not enough Buttons in ScrollBox."));

	checkf(Arr.Num() != 0, TEXT("Error in UUI_BuildingSelection::PopulateScrollBox : Arr is empty."));

#endif

	int count{ 0 };

	for (auto& Element : M)
	{

		if (Element.Value.ComboBoxOptionType.ToString() != ComboBoxOption)
			continue;

		auto Button { Cast <UUI_ConstructionButton> (Arr[count]) };

		if (Button)
		{
			Button->SetButtonText(Element.Key);
			Button->SetVisibility(ESlateVisibility::Visible);
		}

		++count;
	}

	for (int i{ count }; i < Arr.Num(); ++i)
		Arr[i]->SetVisibility(ESlateVisibility::Collapsed);

}

void UUI_ConstructionSelection::ClearScrollBox()
{
	for (auto Element : ScrollBox->GetAllChildren())
	{
		Element->SetVisibility(ESlateVisibility::Collapsed);
	}
}
