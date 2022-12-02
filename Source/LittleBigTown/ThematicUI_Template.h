// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIBuildingButton.h"
#include "UI_BuildingSelection.h"
#include "Building.h"
#include "Components/WidgetSwitcher.h"
#include "Types/SlateEnums.h"

//for definitions
#include "LittleBigTown.h"

#include "ThematicUI_Template.generated.h"

UCLASS(Abstract)
class LITTLEBIGTOWN_API UThematicUI_Template : public UUserWidget
{
	GENERATED_BODY()

public : 

	virtual void NativeConstruct() override;

	// Update Thematic Widget with Map of Themes submitted by a Constructible Slot
	// Check if Map in parameters as same Num() as Child Buttons of the VerticalBox
	// Called in BPPlayerController (in Editor)
	UFUNCTION(BlueprintCallable)
		void UpdateAndDisplayInterface(TEnumAsByte <ESlotSize> SlotSize);

protected :

	UPROPERTY(meta = (BindWidget))
		UWidgetSwitcher* WidgetSwitcher;


};
