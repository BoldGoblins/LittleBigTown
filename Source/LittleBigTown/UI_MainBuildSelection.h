// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIBuildingButton.h"
#include "Components/ScrollBox.h"

#include "UI_MainBuildSelection.generated.h"


// --------------------------------------- CLASS NOT IN USE --------------------------------------- 

UCLASS(Abstract)
class LITTLEBIGTOWN_API UUI_MainBuildSelection : public UUserWidget
{
	GENERATED_BODY()
/*
public :

	// Reset ScrollBox and Buttons
	// Set this Widget as OpennedBuildingWidget in Editor
	UFUNCTION(BlueprintCallable)
		void ResetScrollBox(bool ResetScroll = true);

	void SetLastButtonClicked(UUIBuildingButton* Button);

	UScrollBox* GetScrollBox() { return ScrollBox; }

protected :

	UPROPERTY(BlueprintReadWrite)
		class UUIBuildingButton* LastButtonClicked;

	UPROPERTY(meta = (BindWidget))
		class UScrollBox* ScrollBox;
*/
};
