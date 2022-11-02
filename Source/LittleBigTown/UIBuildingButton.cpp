// Fill out your copyright notice in the Description page of Project Settings.

#include "UIBuildingButton.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Styling/SlateTypes.h"

void UUIBuildingButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	Button->AddChild(TextBlock);
	TextBlock->SetText(FText::FromName(Name));

	Button->SetStyle(BasicStyle);

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

void UUIBuildingButton::SetButtonDisabled(bool IsDisabled, bool ThematicWidget)
{
	ButtonDisabled = IsDisabled;

	if (IsDisabled)
	{
		Button->SetStyle(OnDisabledStyle);

		if (!ThematicWidget)
			Button->SetVisibility(ESlateVisibility::Collapsed);

	}
	else
	{
		Button->SetStyle(BasicStyle);
	}
}

bool UUIBuildingButton::GetButtonClicked()
{
	return ButtonClicked;
}

