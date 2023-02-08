// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LittleBigTown/UserInterface/Templates/BGUserWidget.h"

#include "BGMenuSelection.generated.h"

// Base Class for Menu with Buttons in box (Scroll/Vertical/Horizontal) :
// LastButtonClicked (Button must be of class UBGButton !)

UCLASS()
class LITTLEBIGTOWN_API UBGMenuSelection : public UBGUserWidget
{
	GENERATED_BODY()

public : 

	UBGButton* ResetLastButtonClicked();

	UFUNCTION()
		virtual void ResetWidget(ESlateVisibility Visible);

	UFUNCTION()
		virtual void OnItemClicked(UBGButton* Button);

protected : 

	TWeakObjectPtr <class UBGButton> LastButtonClicked;
	
};
