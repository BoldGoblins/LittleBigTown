// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Building.h"
#include "Components/ScrollBox.h"
#include "LittleBigTown.h"

#include "UI_BuildingSelection.generated.h"


UCLASS(Abstract)
class LITTLEBIGTOWN_API UUI_BuildingSelection : public UUserWidget
{
	GENERATED_BODY()

public:

	// Handle ButtonInteraction of (BuildingSelection Widget)
	// Broadcast Delegate (ConstructionProposition in MainPlayerController) when button is Clicked
	// Called from BP BuildingButton Template in Editor
	UFUNCTION(BlueprintCallable)
		void ButtonInteraction(UUIBuildingButton* Button);

	// Reset ButtonClicked and ScrollToStart if option is true
	UFUNCTION(BlueprintCallable)
		void ResetScrollBox(bool ResetScroll = true);

	// Set visibility of all children of the ScrollBox on Collapsed but does not change Button's Text in any ways
	void ClearScrollBox();

	// Check that M does not own more index than MAX_SCROLLBOX_BUTTONS
	// For each index of M, set Button Text and Set visibility on Visible
	// Set others buttons remaining in Collapsed
	void PopulateScrollBox(const TMap <FName, FBuildingContainers>& M, const FString& ComboBoxOption);

	void NativeConstruct() override;

protected:

	UPROPERTY(BlueprintReadOnly)
		class AMainPlayerController* PlayerController;

	UPROPERTY(BlueprintReadWrite)
		class UUIBuildingButton* LastButtonClicked;

	UPROPERTY(meta = (BindWidget))
		class UScrollBox* ScrollBox;

};
