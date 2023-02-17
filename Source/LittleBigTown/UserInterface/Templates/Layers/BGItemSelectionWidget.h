// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
// #include "LittleBigTown/UserInterface/Templates/BGUserWidget.h"

#include "LittleBigTown/UserInterface/Templates/BGLayerWidget.h"

#include "BGItemSelectionWidget.generated.h"

// Base Class for Menu with Buttons in box (Scroll/Vertical/Horizontal) :
// LastButtonClicked (Button must be of class UBGButton !)

UCLASS()
class LITTLEBIGTOWN_API UBGItemSelectionWidget : public UBGLayerWidget
{
	GENERATED_BODY()

	UFUNCTION()
		virtual void OnItemClicked(UBGButton* Button);

protected : 

	UBGButton* ResetLastButtonClicked();

	TWeakObjectPtr <class UBGButton> LastButtonClicked;
	
};
