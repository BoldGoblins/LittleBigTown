// Fill out your copyright notice in the Description page of Project Settings.


#include "LittleBigTown/UserInterface/BuildingInfos/UI_General_Infos.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/ProgressBar.h"
#include "Components/VerticalBox.h"
// DEBUG_ONLY
#include "LittleBigTown/Core/Debugger.h"
#include "LittleBigTown/GameSystem/MainPlayerController.h"

void UUI_General_Infos::NativeConstruct()
{

}

void UUI_General_Infos::ResetAllComponents()
{
	const auto ChildrenArray{ VerticalBox->GetAllChildren() };

	// As we don't want first Child (Wealth that would be printed in all cases)
	// -1 for Description
	for (int i{ 1 }; i < ChildrenArray.Num() - 1; ++i)
	{
		if (ChildrenArray[i])
			ChildrenArray[i]->SetVisibility(ESlateVisibility::Hidden);
	}
}
void UUI_General_Infos::UpdateAndDisplayBaseInfos(const struct FBuildingInfosBase& BaseInfos)
{
	// Text :
	TB_Outgoings->SetText(FText::FromString(FString::FromInt(BaseInfos.m_Outgoings)));
	TB_Level->SetText(FText::FromString(FString::FromInt(BaseInfos.m_CurrentLevel)));
	TB_OccupationCount->SetText(FText::FromString(FString::FromInt(BaseInfos.m_OccupationCurrentCount)));
	TB_OccupationMax->SetText(FText::FromString(FString::FromInt(BaseInfos.m_OccupationMaxCount)));
	TB_Wealth->SetText(BaseInfos.GetWealthLevelAsText());
	TB_Description->SetText(BaseInfos.m_Description);

	// ProgressBars :
	PB_Level->SetPercent(0.1f);
	PB_Occupation->SetPercent(float(BaseInfos.m_OccupationCurrentCount) /
		BaseInfos.m_OccupationMaxCount);

	// Visibility :
	HB_Outgoings->SetVisibility(ESlateVisibility::Visible);
	PB_Level->SetVisibility(ESlateVisibility::Visible);
	PB_Occupation->SetVisibility(ESlateVisibility::Visible);
}

void UUI_General_Infos::UpdateAndDisplayInfos(const FBuildingInfosBase& BaseInfos, const FResidentialBuildingInfos& ResInfos)
{
	UpdateAndDisplayBaseInfos(BaseInfos);

	// Text :
	TB_Incomes->SetText(FText::FromString(FString::FromInt(ResInfos.m_TotalIncomes)));
	TB_Satisfaction->SetText(FText::FromString(FString::FromInt(int(ResInfos.m_SatisfactionPercent * 100))));

	// ProgressBars :
	PB_Satisfaction->SetPercent(ResInfos.m_SatisfactionPercent);
	PB_Satisfaction->SetVisibility(ESlateVisibility::Visible);

	// HBoxes :
	HB_Incomes->SetVisibility(ESlateVisibility::Visible);
	HB_Level->SetVisibility(ESlateVisibility::Visible);
	HB_Occupation->SetVisibility(ESlateVisibility::Visible);
	HB_Satisfaction->SetVisibility(ESlateVisibility::Visible);
}

