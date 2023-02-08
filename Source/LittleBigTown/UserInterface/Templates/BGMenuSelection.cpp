// Fill out your copyright notice in the Description page of Project Settings.


#include "LittleBigTown/UserInterface/Templates/BGMenuSelection.h"

#include "LittleBigTown/UserInterface/Templates/BGButton.h"


UBGButton* UBGMenuSelection::ResetLastButtonClicked()
{
	const auto ToRet { LastButtonClicked.Get() };

	if (LastButtonClicked.IsValid())
	{
		LastButtonClicked.Get()->SetButtonClicked(false);
		LastButtonClicked.Reset();
	}
	
	return ToRet;
}

void UBGMenuSelection::ResetWidget(ESlateVisibility Visible)
{
	if(Visible == ESlateVisibility::Collapsed || Visible == ESlateVisibility::Hidden)
		ResetLastButtonClicked();
}

void UBGMenuSelection::OnItemClicked(UBGButton* Button)
{
	unimplemented();
}
