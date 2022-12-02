// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Building.h"

#include "UI_BuildingSelection.generated.h"


UCLASS(Abstract)
class LITTLEBIGTOWN_API UUI_BuildingSelection : public UUserWidget
{
	GENERATED_BODY()

public :

	// Handle ButtonInteraction of (BuildingSelection Widget)
	// Broadcast Delegate (ConstructionProposition in MainPlayerController) when button is Clicked
	// Called from BP BuildingButton Template in Editor
	UFUNCTION(BlueprintCallable)
		void ButtonInteraction (UUIBuildingButton* Button);

	// Reset ScrollBox and Buttons
	// Set this Widget as OpennedBuildingWidget in Editor
	UFUNCTION(BlueprintCallable)
		void ResetScrollBox(bool ResetScroll = true);

	// void PopulateScrollBox(const TMap <FName, TSubclassOf <ABuilding>>& BuildingMap, int ComboBoxIndex);

	void NativeConstruct() override;

protected :
	
	UPROPERTY(BlueprintReadOnly)
		class AMainPlayerController* PlayerController;

	UPROPERTY(BlueprintReadWrite)
		class UUIBuildingButton* LastButtonClicked;

	UPROPERTY(meta = (BindWidget))
		class UScrollBox* ScrollBox;
	
};
