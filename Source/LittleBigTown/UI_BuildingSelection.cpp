// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_BuildingSelection.h"

#include "MainPlayerController.h"
#include "UIBuildingButton.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ScrollBox.h"


void UUI_BuildingSelection::NativeConstruct()
{
	Super::NativeConstruct();

	// For beeing able to Broadcast delegate On Construction Launched
	PlayerController = Cast <AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
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

void UUI_BuildingSelection::ResetInterface(bool ResetScroll)
{
	if (LastButtonClicked)
		LastButtonClicked->SetButtonClicked(false);

	if (ScrollBox && ResetScroll)
		ScrollBox->ScrollToStart();

	// As we called this function before displaying this widget
	if (PlayerController)
		PlayerController->SetOpennedBuildingWidget(this);
}