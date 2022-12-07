// Fill out your copyright notice in the Description page of Project Settings.

#include "UIBuildingButton.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Styling/SlateTypes.h"
#include "ThematicUI_Template.h"

void UUIBuildingButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	Button->AddChild(TextBlock);
	// TextBlock->SetText(FText::FromName(Name));

	Button->SetStyle(BasicStyle);

}

void UUIBuildingButton::OnClickedHandle()
{
	auto PC { Cast <AMainPlayerController> (UGameplayStatics::GetPlayerController(GetWorld(), 0)) };

	if (PC)
	{
		auto BuildingWidget{ PC->GetConstructionWidget()->GetBuildingSelectionWidget() };
		BuildingWidget->ButtonInteraction(this);
	}
}

void UUIBuildingButton::NativeConstruct()
{
	Super::NativeConstruct();

	Button->OnClicked.AddUniqueDynamic(this, &ThisClass::OnClickedHandle);
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

