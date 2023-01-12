// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Main.h"
#include "LittleBigTown/GameSystem/MainPlayerController.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "LittleBigTown/GameSystem/MainGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanel.h"
#include "Components/HorizontalBox.h"
#include "LittleBigTown/UserInterface/UI_Demand.h"

// InfoBuildingHandle
#include "LittleBigTown/Actors/Building.h"
#include "LittleBigTown/Actors/ResidentialBuilding.h"
#include "UI_BuildingInfos.h"
// DEBUG_ONLY
#include "LittleBigTown/Core/Debugger.h"

void UUI_Main::NativeConstruct()
{
	Super::NativeConstruct();

	TB_Time->SetText(FText::FromString(FIRST_TIME));
	TB_Month->SetText(FText::FromString(FIRST_MONTH));
	TB_Year->SetText(FText::FromString(FIRST_YEAR));

	GameState = Cast <AMainGameState>(UGameplayStatics::GetGameState(GetWorld()));;
	GameState->OnNewYearDelegate.AddDynamic(this, &ThisClass::SetClockYearUpdate);
	GameState->OnNewMonthDelegate.AddDynamic(this, &ThisClass::SetClockMonthUpdate);
	GameState->OnNewHourDelegate.AddDynamic(this, &ThisClass::SetClockHourUpdate);
	GameState->OnNewMinuteDelegate.AddDynamic(this, &ThisClass::SetClockMinuteUpdate);

	// Set MainWidget in PC
	const auto PC { Cast <AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)) };

	if (PC)
		PC->SetMainWidget(this);

	BuildingInfosWidget = Cast <UUI_BuildingInfos>(CanvasPanel->GetChildAt(BUILDING_INFOS_POS));

	DemandWidget = Cast <UUI_Demand> (HB_Low->GetChildAt(DEMAND_WIDGET_POS));
	DemandWidget->UpdateDemand(GameState->GetResidentialDemand(WealthLevels::Poor),
		GameState->GetResidentialDemand(WealthLevels::Middle), GameState->GetResidentialDemand(WealthLevels::Rich));
}

void UUI_Main::GamePropertiesDisplay()
{
	// ProgressBars :
	DemandWidget->UpdateDemand(GameState->GetResidentialDemand(WealthLevels::Poor),
		GameState->GetResidentialDemand(WealthLevels::Middle), GameState->GetResidentialDemand(WealthLevels::Rich));

	// Text :
	TB_Population->SetText(FText::FromString(FString::FormatAsNumber(GameState->GetPopulationCount())));

	TB_PopVariation->SetText(FText::FromString((GameState->GetPopulationVariation() >= 0 ? TEXT("+") : TEXT("")) +
		FString::FormatAsNumber(GameState->GetPopulationVariation())));

	TB_TotalMoney->SetText(FText::FromString(FString::FormatAsNumber(GameState->GetTotalMoney()) + TEXT(" $")));

	TB_Incomes->SetText(FText::FromString((GameState->GetIncomes() >= 0 ? TEXT("+") : TEXT("")) +
		FString::FormatAsNumber(GameState->GetIncomes()) + TEXT(" $")));

	// Color :
	GameState->GetPopulationVariation() < 0 ? TB_PopVariation->SetColorAndOpacity(FONT_COLOR_RED) : TB_PopVariation->SetColorAndOpacity(FONT_COLOR_GREEN);
	GameState->GetTotalMoney() < 0 ? TB_TotalMoney->SetColorAndOpacity(FONT_COLOR_RED) : TB_TotalMoney->SetColorAndOpacity(FONT_COLOR_WHITE);
	GameState->GetIncomes() < 0 ? TB_Incomes->SetColorAndOpacity(FONT_COLOR_RED) : TB_Incomes->SetColorAndOpacity(FONT_COLOR_GREEN);
}

void UUI_Main::SetClockMinuteUpdate(int32 Minute)
{
	// ---------------------- TIME MANAGEMENT ---------------------- 

	FString Temp { TB_Time->GetText().ToString() };
	FString S {};

	if (Minute < 10)
		S.AppendChar('0');

	S.AppendInt(Minute);

	Temp[3] = S[0];
	Temp[4] = S[1];

	TB_Time->SetText(FText::FromString(Temp));

	// 1439 is maximum amont of minutes per days
	ProgressBar->SetPercent(float(GameState->GetGameClock().GetHour() * 60 + Minute) / 1439);

	// Update monthly properties (population, incomes from Residential/Offices, ...)
	GamePropertiesDisplay();
}

void UUI_Main::SetClockHourUpdate(int32 Hour)
{
	FString Temp { TB_Time->GetText().ToString() };
	FString S{};

	if (Hour < 10)
		S.AppendChar('0');

	S.AppendInt(Hour);

	Temp[0] = S[0];
	Temp[1] = S[1];

	TB_Time->SetText(FText::FromString(Temp));
}

void UUI_Main::SetClockMonthUpdate(int32 Month)
{
	TB_Month->SetText(GameState->GetGameClockMonth());
}

void UUI_Main::SetClockYearUpdate(int32 Year)
{
	TB_Year->SetText(FText::FromString(FString::FromInt(Year)));
}


void UUI_Main::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

}
