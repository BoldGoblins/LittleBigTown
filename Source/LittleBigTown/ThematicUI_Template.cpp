#include "ThematicUI_Template.h"

#include "Components/VerticalBox.h"
// #include "UI_ComboBoxBuildSelection.h"
// #include "Components/WidgetSwitcher.h"
// #include "Components/ScrollBox.h"
#include "Components/ComboBoxString.h"

#include "Kismet/GameplayStatics.h"
// #include "MainPlayerController.h"

#include "ConstructibleSlot.h"

/*
void UThematicUI_Template::NativeConstruct()
{
	Super::NativeConstruct();
}

void UThematicUI_Template::UpdateAndDisplayInterface(TEnumAsByte <ESlotSize> SlotSize)
{
#ifdef DEBUG_ONLY

	checkf(SlotSize != ESlotSize::DefaultSizeEnum, TEXT("Error in UUThematicUI_Template::UpdateAndDisplayInterface : SlotSize == ESlotSize::DefaultSizeEnum."));

#endif

	if (SlotSize == ESlotSize::DefaultSizeEnum)
		return;

	if (SlotSize==ESlotSize::NoSize)
		WidgetSwitcher->SetActiveWidgetIndex(0);

	else
		WidgetSwitcher->SetActiveWidgetIndex(SlotSize);

	auto Element = Cast <UUI_ComboBoxBuildSelection>(WidgetSwitcher->GetActiveWidget());

	if (Element)
		Element->ResetComboBox();

	this->SetVisibility(ESlateVisibility::Visible);
}
*/
void UThematicUI_Template::UpdateFromNewSelection(FString String, ESelectInfo::Type Type)
{
	BuildingSelectionWidget->PopulateScrollBox(GameMode->GetBuildingsMap(LastSlotType, LastSlotSize), String);
}
void UThematicUI_Template::NativeConstruct()
{
	Super::NativeConstruct();

	// PlayerController = Cast <AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	auto Widget{ Cast <UUI_BuildingSelection> (VerticalBox->GetChildAt(BUILDING_SELECTION_WIDGET_POS)) };

	if (Widget)
		BuildingSelectionWidget = Widget;

	GameMode = Cast <AMainGameMode> (UGameplayStatics::GetGameMode(GetWorld()));

	ComboBox->OnSelectionChanged.AddDynamic(this, &ThisClass::UpdateFromNewSelection);
}

TArray<FString> UThematicUI_Template::CheckTypeAndSize(TEnumAsByte<ESlotSize> SlotSize, TEnumAsByte<ESlotType> SlotType)
{
#ifdef DEBUG_ONLY

	checkf(SlotType != ESlotType::DefaultTypeEnum,
		TEXT("Erreur in UThematicUI_Template::UpdateAndDisplayInterface : incorrect Type."));
#endif 

	TArray <FString> Arr;

	switch (SlotType)
	{
	case ESlotType::Residential:
#ifdef DEBUG_ONLY

		checkf(SlotSize != ESlotSize::NoSize && SlotSize != ESlotSize::DefaultSizeEnum,
			TEXT("Erreur in UThematicUI_Template::UpdateAndDisplayInterface : Size incompatible with Type."));
#endif 

		switch (SlotSize)
		{
		case ESlotSize::Large: return { "Riche" };

		default: 
			return { "Pauvre", "Moyen", "Riche" };
		}

	case ESlotType::Commercial:
#ifdef DEBUG_ONLY

		checkf(SlotSize != ESlotSize::NoSize && SlotSize != ESlotSize::DefaultSizeEnum,
			TEXT("Erreur in UThematicUI_Template::UpdateAndDisplayInterface : Size incompatible with Type."));
#endif 
		return { TEXT("Détaillants et services"), "Nourriture et boissons", "Divertissement", "Culture et tourisme" };

	case ESlotType::Offices:
#ifdef DEBUG_ONLY

		checkf(SlotSize != ESlotSize::NoSize && SlotSize != ESlotSize::DefaultSizeEnum,
			TEXT("Erreur in UThematicUI_Template::UpdateAndDisplayInterface : Size incompatible with Type."));
#endif 
		switch (SlotSize)
		{
		case ESlotSize::Medium: return { "Bureaux", "Informatique" };
		case ESlotSize::Large: return { "Bureaux", "Informatique", TEXT("Sièges sociaux") };
		default: return { "Bureaux" };
		}

	case ESlotType::Industrial:
#ifdef DEBUG_ONLY

		checkf(SlotSize == ESlotSize::NoSize,
			TEXT("Erreur in UThematicUI_Template::UpdateAndDisplayInterface : Size incompatible with Type."));
#endif 
		return { "Manufacture", TEXT("Agro-alimentaire"), "Pharmaceutique", TEXT("Electroménager"), "Luxe" };

	default: return {""};
	}
}

void UThematicUI_Template::UpdateAndDisplayInterface(TEnumAsByte<ESlotSize> SlotSize, TEnumAsByte<ESlotType> SlotType)
{
	LastSlotType = SlotType;
	LastSlotSize = SlotSize;

	MyPersonalLibrary::AddOptionsToComboBoxString(ComboBox, CheckTypeAndSize(SlotSize, SlotType));
	BuildingSelectionWidget->ResetScrollBox(true);
	BuildingSelectionWidget->ClearScrollBox();
	BuildingSelectionWidget->PopulateScrollBox(GameMode->GetBuildingsMap(SlotType, SlotSize), ComboBox->GetSelectedOption());
	this->SetVisibility(ESlateVisibility::Visible);
}

