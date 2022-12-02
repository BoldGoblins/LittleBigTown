#include "ThematicUI_Template.h"

#include "Components/VerticalBox.h"
#include "UI_ComboBoxBuildSelection.h"
#include "Components/WidgetSwitcher.h"
#include "Components/ScrollBox.h"
#include "Components/ComboBoxString.h"

#include "Kismet/GameplayStatics.h"
#include "MainGameMode.h"

#include "UI_BuildingSelection.h"

#include "ConstructibleSlot.h"


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

