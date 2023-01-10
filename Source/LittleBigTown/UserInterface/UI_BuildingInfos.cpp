// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_BuildingInfos.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/ProgressBar.h"
#include "Components/VerticalBox.h"
// Building ref
#include "LittleBigTown/Actors/ResidentialBuilding.h"
// DEBUG_ONLY
#include "LittleBigTown/Core/Debugger.h"


void UUI_BuildingInfos::NativeConstruct()
{

}
// Building passed from BP_MainPlayerController
// UpdateAndDisplay is inside the if (Type) because we need to trigger the good overload version of this function
// Essayer d'automatiser opération au sein d'une fonction private ?
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
		CurrResBuilding->OnResBuildingInfosChangedDelegate.AddDynamic(this, &ThisClass::UpdateAndDisplayInfos);
		UpdateAndDisplayInfos(CurrResBuilding.Get()->GetInfosBase(), CurrResBuilding.Get()->GetInfosResidential());
	}

	SetVisibility(ESlateVisibility::Visible);
}
// Check all object ptr for each types
void UUI_BuildingInfos::HideBuildingInfo()
{
	SetVisibility(ESlateVisibility::Collapsed);
	
	if (CurrResBuilding.IsValid())
	{
		CurrResBuilding->OnResBuildingInfosChangedDelegate.RemoveDynamic(this, &ThisClass::UpdateAndDisplayInfos);
		CurrResBuilding.Reset();
	}
}

void UUI_BuildingInfos::ResetAllComponents()
{
	const auto ChildrenArray { VerticalBox->GetAllChildren() };

	// As we don't want first Child (SizeBox / Name that would be printed in all cases)
	// We don't want last index too (Spacer will be displayed anyway)
	for (int i{ 1 }; i < ChildrenArray.Num() - 1; ++i)
	{
		if (ChildrenArray[i])
			ChildrenArray[i]->SetVisibility(ESlateVisibility::Visible);
	}
}
void UUI_BuildingInfos::UpdateAndDisplayBaseInfos(const struct FBuildingInfosBase& BaseInfos)
{
	// Text :
	TB_Name->SetText(BaseInfos.m_Name);
	TB_Outgoings->SetText(FText::FromString(FString::FromInt(BaseInfos.m_Outgoings)));
	TB_Level->SetText(FText::FromString(FString::FromInt(BaseInfos.m_CurrentLevel)));
	TB_OccupationCount->SetText(FText::FromString(FString::FromInt(BaseInfos.m_OccupationCurrentCount)));
	TB_OccupationMax->SetText(FText::FromString(FString::FromInt(BaseInfos.m_OccupationMaxCount)));

	// ProgressBars :
	PB_Level->SetPercent(0.1f);
	PB_Occupation->SetPercent(float(BaseInfos.m_OccupationCurrentCount) /
		BaseInfos.m_OccupationMaxCount);
}

void UUI_BuildingInfos::UpdateAndDisplayInfos(const FBuildingInfosBase & BaseInfos, const FResidentialBuildingInfos& ResInfos)
{
	ResetAllComponents();
	UpdateAndDisplayBaseInfos(BaseInfos);

	// Text :
	TB_Incomes->SetText(FText::FromString(FString::FromInt(ResInfos.m_Incomes)));
	TB_Satisfaction->SetText(FText::FromString(FString::FromInt(int (ResInfos.m_SatisfactionPercent * 100))));

	// ProgressBars :
	PB_Satisfaction->SetPercent(ResInfos.m_SatisfactionPercent);

	// HBoxes :
	HB_Incomes->SetVisibility(ESlateVisibility::Visible);
	HB_Level->SetVisibility(ESlateVisibility::Visible);
	HB_Occupation->SetVisibility(ESlateVisibility::Visible);
	HB_Outgoings->SetVisibility(ESlateVisibility::Visible);
	HB_Satisfaction->SetVisibility(ESlateVisibility::Visible);
}