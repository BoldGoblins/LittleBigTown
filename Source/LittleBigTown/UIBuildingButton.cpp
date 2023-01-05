// Fill out your copyright notice in the Description page of Project Settings.

#include "UIBuildingButton.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Styling/SlateTypes.h"
#include "ThematicUI_Template.h"
#include "Kismet/GameplayStatics.h"
#include "UI_BuildingSelection.h"
#include "MainPlayerController.h"

void UUIBuildingButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	Button->AddChild(TextBlock);
	Button->SetStyle(BasicStyle);

}

void UUIBuildingButton::NativeConstruct()
{
	Super::NativeConstruct();

	Button->OnClicked.AddUniqueDynamic(this, &ThisClass::OnClickedHandle);

	auto PC{ Cast <AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)) };

	if (PC)
		PlayerController = PC;

}

void UUIBuildingButton::OnClickedHandle()
{

#ifdef DEBUG_ONLY

	checkf(PlayerController,
		TEXT("Error in UUIBuildingButton::OnClickedHandle : PlayerController == nullptr."));

#endif DEBUG_ONLY


	if (!PlayerController)
		return;

	const auto ConstructionWidget { PlayerController->GetConstructionWidget() };

	if (ConstructionWidget)
	{
		const auto SelectionWidget { ConstructionWidget->GetBuildingSelectionWidget() };
		
		if (SelectionWidget)
			SelectionWidget->ButtonInteraction(this);
	}
}

void UUIBuildingButton::SetButtonText(const FName& Text) 
{
	Name = Text;
	TextBlock->SetText(FText::FromName(Name));
}


void UUIBuildingButton::SetButtonClicked(bool IsClicked)
{
	ButtonClicked = IsClicked;

	// Don't change style of a Disabled Button or It'll not look Disabled
	if (ButtonDisabled)
		return;

	if (IsClicked)
		Button->SetStyle(OnClickedStyle);
	else
		Button->SetStyle(BasicStyle);
}

bool UUIBuildingButton::GetButtonClicked()
{
	return ButtonClicked;
}

