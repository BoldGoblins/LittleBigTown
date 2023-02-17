// Fill out your copyright notice in the Description page of Project Settings.


#include "LittleBigTown/UserInterface/Templates/Components/BGButton.h"

#include "Styling/SlateTypes.h"
#include "Components/TextBlock.h"

void UBGButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	Button->SetStyle(BasicStyle);

	if (TextBlock)
	{
		TextBlock->SetText(ButtonText);
		TextBlock->SetFont(Font);
	}

	// Add Optionnal Bindings (like image, text, ... here)
}


void UBGButton::NativeConstruct()
{
	Super::NativeConstruct();

	Button->OnClicked.AddDynamic(this, &ThisClass::OnButtonClicked);
}

void UBGButton::OnButtonClicked()
{
	if (!bButtonClicked && bCanBeSelected)
		SetButtonClicked(true);

	OnBGButtonClicked.Broadcast(this);
}

void UBGButton::SetButtonClicked(bool Clicked)
{
	bButtonClicked = Clicked;
	Clicked ? Button->SetStyle(OnClickedStyle) : Button->SetStyle(BasicStyle);
}

void UBGButton::SetButtonDisabled(bool Disabled)
{
	bButtonDisabled = Disabled;
	Disabled ? Button->SetVisibility(ESlateVisibility::HitTestInvisible) : Button->SetVisibility(ESlateVisibility::Visible);
}
