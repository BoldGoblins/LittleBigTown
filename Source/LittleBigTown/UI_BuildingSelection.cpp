// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_BuildingSelection.h"

#include "MainPlayerController.h"
#include "MainGameMode.h"
#include "UIBuildingButton.h"
#include "Kismet/GameplayStatics.h"
// #include "Components/WidgetSwitcher.h"
// #include "LittleBigTown.h"
// #include "UI_MainBuildSelection.h"
// #include "Building.h"


void UUI_BuildingSelection::NativeConstruct()
{
	Super::NativeConstruct();

	// For beeing able to Broadcast delegate On Construction Launched
	PlayerController = Cast <AMainPlayerController> (UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void UUI_BuildingSelection::ButtonInteraction(UUIBuildingButton* Button)
{
	if (!PlayerController)
		return;

	bool temp { Button->GetButtonClicked() };

	if (!temp)
	{
		Button->SetButtonClicked(true);
		LastButtonClicked = Button;
	}

	Button->SetButtonClicked(true);
	PlayerController->ConstructionPropositionDelegate.Broadcast(Button->GetButtonText());
}

void UUI_BuildingSelection::ResetScrollBox(bool ResetScroll)
{
	if (LastButtonClicked)
		LastButtonClicked->SetButtonClicked(false);

	if (ScrollBox && ResetScroll)
		ScrollBox->ScrollToStart();
}

void UUI_BuildingSelection::PopulateScrollBox(const TMap <FName, FBuildingContainers>& M, const FString& ComboBoxOption)
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

		auto Button { Cast <UUIBuildingButton> (Arr[count]) };

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

void UUI_BuildingSelection::ClearScrollBox()
{
	for (auto Element : ScrollBox->GetAllChildren())
	{
		Element->SetVisibility(ESlateVisibility::Collapsed);
	}
}
