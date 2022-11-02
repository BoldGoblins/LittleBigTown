// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIBuildingButton.h"
//for definitions
#include "LittleBigTown.h"

#include "ThematicUI_Template.generated.h"

UCLASS(Abstract)
class LITTLEBIGTOWN_API UThematicUI_Template : public UUserWidget
{
	GENERATED_BODY()

public : 

	virtual void NativeConstruct() override;

	// Handle Interaction with WidgetButtons in Thematic Widgets
	// Reset interface in Thematic Widgets and Child ComboBox
	// Called in Editor after OnClicked Event Dispatcher from BP ThematicButton
	UFUNCTION(BlueprintCallable)
		void ButtonInteraction(UUIBuildingButton* Button, int SwitcherIndex);

	// Bind on OnVisibilityChanged Delegate (of the whole Thematic Widget)
	// Called by ButtonInteraction too
	// Reset LastButtonClicked and hide WidgetSwitcher
	UFUNCTION(BlueprintCallable)
		void ResetInterface(ESlateVisibility InVisibility);

	// Update Thematic Widget with Map of Themes submitted by a Constructible Slot
	// Check if Map in parameters as same Num() as Child Buttons of the VerticalBox
	// Called in BPPlayerController (in Editor)
	UFUNCTION(BlueprintCallable)
		void UpdateAndDisplayInterface(const TMap <FName, bool> & ConstructionThemes);

protected :

	// Max lateral translation when button is clicked
	UPROPERTY(EditDefaultsOnly)
		float XButtonTranslationMax { X_BUTTON_TRANSLATION_MAX };

	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
		class UVerticalBox* VerticalBox;

	UPROPERTY(BlueprintReadWrite)
		UUIBuildingButton* LastButtonClicked;

};
