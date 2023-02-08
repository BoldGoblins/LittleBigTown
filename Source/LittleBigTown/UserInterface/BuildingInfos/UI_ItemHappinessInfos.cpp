// Fill out your copyright notice in the Description page of Project Settings.


#include "LittleBigTown/UserInterface/BuildingInfos/UI_ItemHappinessInfos.h"
#include "LittleBigTown/UserInterface/BuildingInfos/UI_BuildingInfos.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"
#include "LittleBigTown/GameSystem/MainPlayerController.h"

#define RESIZED_FONT_SIZE 10

void UUI_ItemHappinessInfos::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (Resize)
	{
		TB_SpecialtyName->Font.Size = RESIZED_FONT_SIZE;
		TB_SpecialtyName->SetFont(TB_SpecialtyName->Font);
		TB_SpecialtyCount->Font.Size = RESIZED_FONT_SIZE;
		TB_SpecialtyCount->SetFont(TB_SpecialtyName->Font);
	}
}

void UUI_ItemHappinessInfos::SetSpecialtyName(const FText & Name)
{
	TB_SpecialtyName->SetText(Name);
}

void UUI_ItemHappinessInfos::SetInformations(int32 Count, float Satisfaction)
{
	TB_SpecialtyCount->SetText(FText::FromString(FString::FormatAsNumber(Count)));
	PB_Satisfaction->SetPercent(Satisfaction);
}
const int32 UUI_ItemHappinessInfos::GetCount() const
{
	return FCString::Atoi(*TB_SpecialtyCount->GetText().ToString());
}