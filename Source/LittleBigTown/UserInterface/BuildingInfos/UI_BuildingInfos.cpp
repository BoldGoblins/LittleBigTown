// WIDGET BUILDING INFOS :
// Ce Widget est responsable de la gestion du WeakPtr sur le Building que l'on veut Display
// Ce ptr est transmis aux autres layers afin que le Widget Parent se réserve la possibilité de le Reset


#include "UI_BuildingInfos.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/HorizontalBox.h"

#include "LittleBigTown/UserInterface/Templates/Components/BGButton.h"
#include "LittleBigTown/UserInterface/BuildingInfos/UI_General_Infos.h"
// Building ref
#include "LittleBigTown/Actors/ResidentialBuilding.h"
// DEBUG_ONLY
#include "LittleBigTown/Core/Debugger.h"
#include "LittleBigTown/GameSystem/MainPlayerController.h"

#define WIDGET_GENERAL_POS 0


void UUI_BuildingInfos::NativeConstruct()
{
	MainPlayerController = Cast <AMainPlayerController> (UGameplayStatics::GetPlayerController(GetWorld(), 0));
	MainPlayerController->SetBuildingInfosWidget(this);

	for (const auto & Button : ButtonBox->GetAllChildren())
		Cast <UBGButton>(Button)->OnBGButtonClicked.AddDynamic(this, &ThisClass::ButtonClicked);

	for (const auto & Widget : WidgetSwitcher->GetAllChildren())
	{
		const auto & Layer { Cast <UBGLayerWidget>(Widget) };

		if(Layer)
			Layers.Add(Cast <UBGLayerWidget>(Widget));
	}

#ifdef DEBUG_ONLY

	checkf(ButtonBox->GetChildrenCount() == WidgetSwitcher->GetChildrenCount(),
		TEXT("Error in UUI_BuildingInfos::NativeConstruct, Button Count in ButtonBox != Index Index Count in WidgetSwitcher."));

	checkf(WidgetSwitcher->GetChildrenCount() == Layers.Num(), TEXT("Error in UUI_BuildingInfos::NativeConstruct, Cast Layer child has failed."));

#endif
}

// Building passed from BP_MainPlayerController (Click Handle)
// UpdateAndDisplay is inside the if IsA(Type) because we need to trigger the good overload version of this function
void UUI_BuildingInfos::NewDisplayBuildingInfos(ABuilding* Building)
{

#ifdef DEBUG_ONLY

	checkf(Building, TEXT("Error in : UUI_BuildingInfos::NewDisplayBuildingInfos, Building == nullptr"));

#endif

	DisplayedBuilding = Building;

	if (!DisplayedBuilding.IsValid())
		return;

	if (DisplayedBuilding.Get()->IsA <AResidentialBuilding>())
	{
		const auto ResidentialBuilding { Cast <AResidentialBuilding>(Building) };

		if (!ResidentialBuilding)
			return;

		ResidentialBuilding->OnResBuildingInfosChangedDelegate.AddDynamic(this, &ThisClass::Update);
		WidgetSwitcher->SetActiveWidgetIndex(WIDGET_GENERAL_POS);
	}

	TB_Name->SetText(DisplayedBuilding.Get()->GetInfosBase().m_Name);
	TB_Owner->SetText(DisplayedBuilding.Get()->GetInfosBase().m_Owner);
	TB_District->SetText(DisplayedBuilding.Get()->GetInfosBase().m_DistrictName);

	Layers[WIDGET_GENERAL_POS]->SetInformations(DisplayedBuilding.Get());
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

// Called from BP_MainPlayerController Click Handle
void UUI_BuildingInfos::Hide()
{
	SetVisibility(ESlateVisibility::Collapsed);

	for (const auto& Layer : Layers)
		Layer->Reset();

	if (!DisplayedBuilding.IsValid())
		return;

	if(DisplayedBuilding.Get()->IsA<AResidentialBuilding> ())
		Cast <AResidentialBuilding> (DisplayedBuilding.Get())->OnResBuildingInfosChangedDelegate.RemoveDynamic(this, &ThisClass::Update);

	DisplayedBuilding.Reset();
}

void UUI_BuildingInfos::Update()
{
	if (!DisplayedBuilding.IsValid())
		return;
		
	Cast <UBGLayerWidget>(WidgetSwitcher->GetActiveWidget())->SetInformations(DisplayedBuilding.Get(), false);
}

void UUI_BuildingInfos::ButtonClicked(UBGButton* Button)
{
	Cast <UBGLayerWidget>(WidgetSwitcher->GetChildAt(ButtonBox->GetChildIndex(Button)))->SetInformations(DisplayedBuilding.Get(), true);
	WidgetSwitcher->SetActiveWidgetIndex(ButtonBox->GetChildIndex(Button));
}
