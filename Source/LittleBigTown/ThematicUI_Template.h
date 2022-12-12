// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIBuildingButton.h"
#include "Components/VerticalBox.h"
#include "MainPlayerController.h"
#include "UI_BuildingSelection.h"
#include "MainGameMode.h"
#include "Types/SlateEnums.h"
#include "LittleBigTown.h"

#include "ThematicUI_Template.generated.h"

#define BUILDING_SELECTION_WIDGET_POS 3
#define COMBOBOX_OPTIONS_NUM 15

// Delegate signature
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FConstructionPropositionSignature, FName, BuildingName);

UCLASS(Abstract)
class LITTLEBIGTOWN_API UThematicUI_Template : public UUserWidget
{
	GENERATED_BODY()

public :

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
		void UpdateAndDisplayInterface(TEnumAsByte<ESlotType> SlotType, TEnumAsByte<ESlotSize> SlotSize);

	UFUNCTION(BlueprintCallable)
	void UpdateFromNewSelection(FString String, ESelectInfo::Type Type);

	UFUNCTION(BlueprintCallable)
		UUI_BuildingSelection* GetBuildingSelectionWidget() { return BuildingSelectionWidget; }

	TArray <FString> CheckTypeAndSize(TEnumAsByte <ESlotSize> SlotSize, TEnumAsByte <ESlotType> SlotType);

	// Delegate signature
	UPROPERTY(BlueprintAssignable)
		FConstructionPropositionSignature ConstructionPropositionDelegate;

protected : 

	UPROPERTY(meta = (BindWidget))
		UVerticalBox* VerticalBox;

	UPROPERTY(meta = (BindWidget))
		class UComboBoxString* ComboBox;

	UPROPERTY(BlueprintReadOnly)
		UUI_BuildingSelection* BuildingSelectionWidget;
	/*
	UPROPERTY(BlueprintReadOnly)
		AMainPlayerController* PlayerController;
*/
	AMainGameMode* GameMode;

	UPROPERTY(BlueprintReadOnly)
		TEnumAsByte <ESlotType> LastSlotType;

	UPROPERTY(BlueprintReadOnly)
		TEnumAsByte <ESlotSize> LastSlotSize;

	UPROPERTY(EditDefaultsOnly)
		TArray <FText> ComboBoxOptions;


};
