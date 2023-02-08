// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_BuildingInfos.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"

#include "LittleBigTown/UserInterface/BuildingInfos/UI_General_Infos.h"
// Building ref
#include "LittleBigTown/Actors/ResidentialBuilding.h"
// DEBUG_ONLY
#include "LittleBigTown/Core/Debugger.h"
#include "LittleBigTown/GameSystem/MainPlayerController.h"

#define WIDGET_GENERAL_POS 0
#define WIDGET_HAPPINESS_POS 1


void UUI_BuildingInfos::NativeConstruct()
{
	const auto PC { Cast <AMainPlayerController> (UGameplayStatics::GetPlayerController(GetWorld(), 0)) };

	if (PC)
		PC->SetBuildingInfosWidget(this);	

	GeneralInfosWidget = Cast <UUI_General_Infos> (WidgetSwitcher->GetChildAt(WIDGET_GENERAL_POS));
	HappinessInfosWidget = Cast <UUI_HappinessInfos> (WidgetSwitcher->GetChildAt(WIDGET_HAPPINESS_POS));

	B_General->OnClicked.AddDynamic(this, &ThisClass::GeneralClicked);
	B_Happiness->OnClicked.AddDynamic(this, &ThisClass::HappinessClicked);
}
// Building passed from BP_MainPlayerController (Click Handle)
// UpdateAndDisplay is inside the if IsA(Type) because we need to trigger the good overload version of this function
void UUI_BuildingInfos::NewDisplayBuildingInfos(ABuilding* Building)
{

#ifdef DEBUG_ONLY

	checkf(Building, TEXT("Error in : UUI_BuildingInfos::NewDisplayBuildingInfos, Building == nullptr"));

#endif

	if (!Building)
		return;

	if (Building->IsA <AResidentialBuilding>())
	{
		const auto ResidentialBuilding { Cast <AResidentialBuilding>(Building) };

		if (!ResidentialBuilding)
			return;

		CurrResBuilding = ResidentialBuilding;
		CurrResBuilding->OnResBuildingInfosChangedDelegate.AddDynamic(this, &ThisClass::BuildingInfosUpdated);

		// Mettre plus tard dans une fonction dédiée ?
		TB_Name->SetText(CurrResBuilding.Get()->GetInfosBase().m_Name);
		TB_Owner->SetText(CurrResBuilding.Get()->GetInfosBase().m_Owner);
		// Ca aussi...
		GeneralInfosWidget->UpdateAndDisplayInfos(CurrResBuilding.Get()->GetInfosBase(), CurrResBuilding.Get()->GetInfosResidential());
		WidgetSwitcher->SetActiveWidgetIndex(WIDGET_GENERAL_POS);
	}

	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}
// Called from BP_MainPlayerController Click Handle
void UUI_BuildingInfos::Hide()
{
	SetVisibility(ESlateVisibility::Collapsed);
	GeneralInfosWidget->ResetAllComponents();
	HappinessInfosWidget->ResetWidget(ESlateVisibility::Collapsed);

	if (CurrResBuilding.IsValid())
	{
		CurrResBuilding->OnResBuildingInfosChangedDelegate.RemoveDynamic(this, &ThisClass::BuildingInfosUpdated);
		CurrResBuilding.Reset();
	}
}

void UUI_BuildingInfos::BuildingInfosUpdated()
{
	// Checker quel ptr est non null, pour décider de quelles infos envoyer, peut être plus besoin de passer tous les params en delegate
	// Ou sinon définir un nouveau type dans la struct FBuildingInfosBase avec une enum pour le type de bâtiment
	if (CurrResBuilding.IsValid())
	{
		if (WidgetSwitcher->GetActiveWidgetIndex() == 0)
			GeneralInfosWidget->UpdateAndDisplayInfos(CurrResBuilding.Get()->GetInfosBase(), CurrResBuilding.Get()->GetInfosResidential());

		else if (WidgetSwitcher->GetActiveWidgetIndex() == 1)
			HappinessInfosWidget->MainDisplay(CurrResBuilding.Get(), false);
	}

}

void UUI_BuildingInfos::GeneralClicked()
{
	if (CurrResBuilding.IsValid())
		GeneralInfosWidget->UpdateAndDisplayInfos(CurrResBuilding.Get()->GetInfosBase(), CurrResBuilding.Get()->GetInfosResidential());

	WidgetSwitcher->SetActiveWidgetIndex(WIDGET_GENERAL_POS);
}

void UUI_BuildingInfos::HappinessClicked()
{
	if (CurrResBuilding.IsValid())
		HappinessInfosWidget->MainDisplay(CurrResBuilding.Get(), true);

	WidgetSwitcher->SetActiveWidgetIndex(WIDGET_HAPPINESS_POS);
}
