// Fill out your copyright notice in the Description page of Project Settings.

#include "UI_ConstructionButton.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Styling/SlateTypes.h"
#include "UI_ConstructionMain.h"
#include "Kismet/GameplayStatics.h"
#include "UI_ConstructionSelection.h"
#include "LittleBigTown/GameSystem/BGMainPlayerController.h"
// DEBUG_ONLY
#include "LittleBigTown/Core/Debugger.h"

void UUI_ConstructionButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	Button->AddChild(TextBlock);
	Button->SetStyle(BasicStyle);

}

void UUI_ConstructionButton::NativeConstruct()
{
	Super::NativeConstruct();

	Button->OnClicked.AddUniqueDynamic(this, &ThisClass::OnClickedHandle);

	// auto PC{ Cast <AMainPlayerController> (UGameplayStatics::GetPlayerController(GetWorld(), 0)) };

	// if (PC)
		// PlayerController = PC;

}

void UUI_ConstructionButton::OnClickedHandle()
{
	const auto PlayerController { Cast <ABGMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)) };

#ifdef DEBUG_ONLY

	checkf(IsValid(PlayerController),
		TEXT("Error in UUIBuildingButton::OnClickedHandle : PlayerController == nullptr."));

#endif


	if (!IsValid(PlayerController))
		return;

	const auto ConstructionWidget { PlayerController->GetConstructionWidget() };

	if (ConstructionWidget)
	{
		const auto SelectionWidget { ConstructionWidget->GetConstructionSelectionWidget() };
		
		if (SelectionWidget)
			SelectionWidget->ButtonInteraction(this);
	}
}

void UUI_ConstructionButton::SetButtonText(const FName& Text)
{
	Name = Text;
	TextBlock->SetText(FText::FromName(Name));
}


void UUI_ConstructionButton::SetButtonClicked(bool IsClicked)
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

bool UUI_ConstructionButton::GetButtonClicked()
{
	return ButtonClicked;
}

