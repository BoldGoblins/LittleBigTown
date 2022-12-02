// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_BuildingSelection.h"

#include "MainPlayerController.h"
#include "MainGameMode.h"
#include "UIBuildingButton.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetSwitcher.h"
#include "Components/ScrollBox.h"
#include "LittleBigTown.h"
#include "UI_MainBuildSelection.h"
#include "Building.h"


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
/*
void UUI_BuildingSelection::PopulateScrollBox(const TMap <FName, TSubclassOf <ABuilding>>& BuildingMap, int ComboBoxIndex)
{
	PlayerController->SetOpennedBuildingWidget(this);
	WidgetSwitcher->SetActiveWidgetIndex(ComboBoxIndex);

	auto ScrollBox { Cast <UUI_MainBuildSelection>(WidgetSwitcher->GetActiveWidget()) };

	for (auto Element : ScrollBox->GetScrollBox()->GetAllChildren())
	{
		auto Button { Cast <UUIBuildingButton>(Element) };

		if (!BuildingMap.Contains(Button->GetButtonText()))
			Element->SetVisibility(ESlateVisibility::Collapsed);
		else
			Element->SetVisibility(ESlateVisibility::Visible);
	}

	ScrollBox->ResetScrollBox();
}*/

void UUI_BuildingSelection::ResetScrollBox(bool ResetScroll)
{
#ifdef DEBUG_ONLY

	checkf(ScrollBox && PlayerController, TEXT("Erreur : UIBuildingSelection::ResetInterface, ScrollBox || PlayerController == nullptr."));

#endif // DEBUG_ONLY


	if (LastButtonClicked)
		LastButtonClicked->SetButtonClicked(false);

	if (ScrollBox && ResetScroll)
		ScrollBox->ScrollToStart();

	// As we called this function before displaying this widget
	if (PlayerController)
	{
		PlayerController->SetOpennedBuildingWidget(this);

		for (auto Element : ScrollBox->GetAllChildren())
		{
			auto Button { Cast <UUIBuildingButton>(Element) };

			const auto Map { PlayerController->GetMainGameMode()->GetBuildingsMap(PlayerController->GetLastSlotType(), PlayerController->GetLastSlotSize())};

			if (!Map.Contains(Button->GetButtonText()))
				Element->SetVisibility(ESlateVisibility::Collapsed);
			else
				Element->SetVisibility(ESlateVisibility::Visible);
		}
	}
}
