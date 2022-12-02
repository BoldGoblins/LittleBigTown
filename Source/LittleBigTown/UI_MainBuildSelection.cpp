// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_MainBuildSelection.h"
#include "UIBuildingButton.h"
#include "Components/ScrollBox.h"


// --------------------------------------- CLASS NOT IN USE --------------------------------------- 
/*
void UUI_MainBuildSelection::ResetScrollBox (bool ResetScroll)
{
#ifdef DEBUG_ONLY

	checkf(ScrollBox, TEXT("Erreur : UUI_MainBuildSelection::ResetScrollBox, ScrollBox == nullptr."));

#endif


	if (LastButtonClicked)
		LastButtonClicked->SetButtonClicked(false);

	if (ScrollBox && ResetScroll)
		ScrollBox->ScrollToStart();
}

void UUI_MainBuildSelection::SetLastButtonClicked(UUIBuildingButton* Button)
{
	if (Button)
		LastButtonClicked = Button;
}*/