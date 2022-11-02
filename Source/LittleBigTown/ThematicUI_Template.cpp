#include "ThematicUI_Template.h"

#include "Components/VerticalBox.h"
#include "UI_ComboBoxBuildSelection.h"
#include "Components/WidgetSwitcher.h"


void UThematicUI_Template::NativeConstruct()
{
	Super::NativeConstruct();

	OnVisibilityChanged.AddUniqueDynamic(this, &UThematicUI_Template::ResetInterface);
}

void UThematicUI_Template::ButtonInteraction(UUIBuildingButton* Button, int SwitcherIndex)
{
	if (!Button)
		return;

	bool temp { Button->GetButtonClicked() };

	ResetInterface(ESlateVisibility::Visible);

	if (!temp)
	{
		Button->SetRenderTranslation(FVector2D(XButtonTranslationMax, 0));
		Button->SetButtonClicked(true);
		LastButtonClicked = Button;

		if (WidgetSwitcher)
		{
			WidgetSwitcher->SetActiveWidgetIndex(SwitcherIndex);

			auto Element = Cast <UUI_ComboBoxBuildSelection> (WidgetSwitcher->GetChildAt(SwitcherIndex));

			if (Element)
				Element->ResetComboBox();
		
			WidgetSwitcher->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UThematicUI_Template::ResetInterface(ESlateVisibility InVisibility)
{
	if (!LastButtonClicked || Visibility != ESlateVisibility::Visible)
		return;

	LastButtonClicked->SetRenderTranslation(FVector2D(0, 0));
	LastButtonClicked->SetButtonClicked(false);

	WidgetSwitcher->SetVisibility(ESlateVisibility::Collapsed);
}

void UThematicUI_Template::UpdateAndDisplayInterface(const TMap <FName, bool> & ConstructionThemes)
{
	auto ButtonArray { VerticalBox->GetAllChildren() };

#ifdef DEBUG_ONLY

	checkf(ButtonArray.Num() == ConstructionThemes.Num(), 
		TEXT("Error in UpdateAndDisplayInterface::UThematicUI_Template) : array size in parameter (%d) != nb of buttons (%d)."), 
		ConstructionThemes.Num(), ButtonArray.Num());

#endif

	if (ConstructionThemes.Num() == 0 || ButtonArray.Num() == 0)
		return;

	for (auto Element : ButtonArray)
	{
		auto Button = Cast <UUIBuildingButton> (Element);

		if (Button)
		{

#ifdef DEBUG_ONLY

			// GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("%d")), Button->GetButtonClicked());

			checkf(ConstructionThemes.Contains(Button->GetButtonText()),
				TEXT("Error in UpdateAndDisplayInterface::UThematicUI_Template : cannot find Button name in map."));

#endif
			if (ConstructionThemes.Contains(Button->GetButtonText()))
				Button->SetButtonDisabled(ConstructionThemes[Button->GetButtonText()] ? false : true, true);
			else
				continue;
		}
	}

	this->SetVisibility(ESlateVisibility::Visible);
}


