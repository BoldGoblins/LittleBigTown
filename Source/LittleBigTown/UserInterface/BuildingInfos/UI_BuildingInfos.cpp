// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_BuildingInfos.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "LittleBigTown/UserInterface/BuildingInfos/UI_General_Infos.h"
// Building ref
#include "LittleBigTown/Actors/ResidentialBuilding.h"
// DEBUG_ONLY
#include "LittleBigTown/Core/Debugger.h"
#include "LittleBigTown/GameSystem/MainPlayerController.h"


void UUI_BuildingInfos::NativeConstruct()
{
	const auto PC { Cast <AMainPlayerController> (UGameplayStatics::GetPlayerController(GetWorld(), 0)) };

	if (PC)
		PC->SetBuildingInfosWidget(this);	

	GeneralInfosWidget = Cast <UUI_General_Infos>(WidgetSwitcher->GetChildAt(WIDGET_GENERAL_POS));
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

		GeneralInfosWidget->UpdateAndDisplayInfos(CurrResBuilding.Get()->GetInfosBase(), CurrResBuilding.Get()->GetInfosResidential());
	}

	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}
// Called from BP_MainPlayerController Click Handle
void UUI_BuildingInfos::HideBuildingInfo()
{
	SetVisibility(ESlateVisibility::Collapsed);
	
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
		GeneralInfosWidget->UpdateAndDisplayInfos(CurrResBuilding.Get()->GetInfosBase(), CurrResBuilding.Get()->GetInfosResidential());
}
