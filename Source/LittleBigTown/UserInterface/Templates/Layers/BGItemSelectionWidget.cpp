// Fill out your copyright notice in the Description page of Project Settings.


#include "LittleBigTown/UserInterface/Templates/Layers/BGItemSelectionWidget.h"

#include "LittleBigTown/UserInterface/Templates/Components/BGButton.h"


UBGButton* UBGItemSelectionWidget::ResetLastButtonClicked()
{
	const auto ToRet { LastButtonClicked.Get() };

	if (LastButtonClicked.IsValid())
	{
		LastButtonClicked.Get()->SetButtonClicked(false);
		LastButtonClicked.Reset();
	}
	
	return ToRet;
}

void UBGItemSelectionWidget::OnItemClicked(UBGButton* Button)
{
	unimplemented();
}
