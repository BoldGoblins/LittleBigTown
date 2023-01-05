#include "ThematicUI_Template.h"

#include "Components/ComboBoxString.h"
#include "Components/CanvasPanel.h"
#include "Kismet/GameplayStatics.h"
#include "ConstructibleSlot.h"
// temp
#include "MainPlayerController.h"

void UThematicUI_Template::UpdateFromNewSelection(FString String, ESelectInfo::Type Type)
{

	BuildingSelectionWidget->ResetScrollBox(true);
	BuildingSelectionWidget->ClearScrollBox();
	BuildingSelectionWidget->PopulateScrollBox(GameMode->GetBuildingsMap(LastSlotType, LastSlotSize), String);

}


void UThematicUI_Template::NativeConstruct()
{
	Super::NativeConstruct();

	GameMode = Cast <AMainGameMode> (UGameplayStatics::GetGameMode(GetWorld()));

	// Set ConstructionWidget into PC
	const auto PC { Cast <AMainPlayerController> (UGameplayStatics::GetPlayerController(GetWorld(), 0)) };

	PC->SetConstructionWidget(this);

	ComboBox->OnSelectionChanged.AddDynamic(this, &ThisClass::UpdateFromNewSelection);

	const auto CanvasChild { CanvasPanel->GetChildAt(BUILDING_VALIDATION_WIDGET_POS) };
	const auto ValidationWidget { Cast <UWidgetBuildingValidation>(CanvasChild) };
	const auto VerticalChild { VerticalBox->GetChildAt(BUILDING_SELECTION_WIDGET_POS) };
	const auto SelectionWidget { Cast <UUI_BuildingSelection>(VerticalChild) };

#ifdef DEBUG_ONLY

	checkf(ValidationWidget, TEXT("Error in UThematicUI_Template::NativeConstruct, ValidationWidget == nullptr"));
	checkf(SelectionWidget, TEXT("Error in UThematicUI_Template::NativeConstruct, SelectionWidget == nullptr"));

#endif

	BuildingValidationWidget = ValidationWidget;
	BuildingSelectionWidget = SelectionWidget;
}

UWidgetBuildingValidation* UThematicUI_Template::GetBuildingValidationWidget()
{

#ifdef DEBUG_ONLY

	checkf(BuildingValidationWidget, TEXT("Error in UThematicUI_Template::GetBuildingValidationWidget, ValidationWidget == nullptr"));

#endif

	return BuildingValidationWidget;
}

UUI_BuildingSelection* UThematicUI_Template::GetBuildingSelectionWidget()
{

#ifdef DEBUG_ONLY

	checkf(BuildingSelectionWidget, TEXT("Error in UThematicUI_Template::GetBuildingSelectionWidget, BuildingSelectionWidget == nullptr"));

#endif

	return BuildingSelectionWidget;
}

TArray<FString> UThematicUI_Template::CheckTypeAndSize(TEnumAsByte<ESlotSize> SlotSize, TEnumAsByte<ESlotType> SlotType)
{

#ifdef DEBUG_ONLY

	checkf(SlotType != ESlotType::DefaultTypeEnum,
		TEXT("Erreur in UThematicUI_Template::UpdateAndDisplayInterface : incorrect Type."));

	checkf(ComboBoxOptions.Num() > 0 && ComboBoxOptions.Num() == COMBOBOX_OPTIONS_NUM,
		TEXT("Error in : UThematicUI_Template::UpdateAndDisplayInterface : ComboBoxOptions Array is empty or does not own enough entries"));
	
#endif 


	switch (SlotType)
	{
	case ESlotType::Residential:

#ifdef DEBUG_ONLY

		checkf(SlotSize != ESlotSize::NoSize && SlotSize != ESlotSize::DefaultSizeEnum,
			TEXT("Erreur in UThematicUI_Template::UpdateAndDisplayInterface : Size incompatible with Type."));
#endif 

		switch (SlotSize)
		{
		case ESlotSize::Large: return { ComboBoxOptions[2].ToString() };

		default: 
			return { ComboBoxOptions[0].ToString(), ComboBoxOptions[1].ToString(), ComboBoxOptions[2].ToString() };
		}

	case ESlotType::Commercial:

#ifdef DEBUG_ONLY

		checkf(SlotSize != ESlotSize::NoSize && SlotSize != ESlotSize::DefaultSizeEnum,
			TEXT("Erreur in UThematicUI_Template::UpdateAndDisplayInterface : Size incompatible with Type."));
#endif 

		return { ComboBoxOptions[3].ToString(), ComboBoxOptions[4].ToString(), ComboBoxOptions[5].ToString(), ComboBoxOptions[6].ToString() };

	case ESlotType::Offices:

#ifdef DEBUG_ONLY

		checkf(SlotSize != ESlotSize::NoSize && SlotSize != ESlotSize::DefaultSizeEnum,
			TEXT("Erreur in UThematicUI_Template::UpdateAndDisplayInterface : Size incompatible with Type."));
#endif 

		switch (SlotSize)
		{
		case ESlotSize::Medium: return { ComboBoxOptions[7].ToString(), ComboBoxOptions[8].ToString() };
		case ESlotSize::Large: return { ComboBoxOptions[7].ToString(), ComboBoxOptions[8].ToString(), ComboBoxOptions[9].ToString() };
		default: return { ComboBoxOptions[7].ToString() };
		}

	case ESlotType::Industrial:

#ifdef DEBUG_ONLY

		checkf(SlotSize == ESlotSize::NoSize,
			TEXT("Erreur in UThematicUI_Template::UpdateAndDisplayInterface : Size incompatible with Type."));
#endif 

		return { ComboBoxOptions[10].ToString(), ComboBoxOptions[11].ToString(), ComboBoxOptions[12].ToString(), ComboBoxOptions[13].ToString(), ComboBoxOptions[14].ToString() };

	default: return {""};
	}
}

void UThematicUI_Template::UpdateAndDisplayInterface(TEnumAsByte<ESlotType> SlotType, TEnumAsByte<ESlotSize> SlotSize)
{
	LastSlotType = SlotType;
	LastSlotSize = SlotSize;

	MyPersonalLibrary::AddOptionsToComboBoxString(ComboBox, CheckTypeAndSize(SlotSize, SlotType));

	BuildingSelectionWidget->ResetScrollBox(true);
	BuildingSelectionWidget->ClearScrollBox();
	BuildingSelectionWidget->PopulateScrollBox(GameMode->GetBuildingsMap(SlotType, SlotSize), ComboBox->GetSelectedOption());

	this->SetVisibility(ESlateVisibility::Visible);
}

