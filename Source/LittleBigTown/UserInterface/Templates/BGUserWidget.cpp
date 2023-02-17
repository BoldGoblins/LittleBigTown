// Fill out your copyright notice in the Description page of Project Settings.


#include "LittleBigTown/UserInterface/Templates/BGUserWidget.h"
#include "LittleBigTown/Actors/Building.h"

void UBGUserWidget::Hide()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UBGUserWidget::Display()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UBGUserWidget::Update()
{
	unimplemented();
}

void UBGUserWidget::ButtonClicked(UBGButton* Button)
{
	unimplemented();
}

void UBGUserWidget::Reset()
{
	unimplemented();
}
