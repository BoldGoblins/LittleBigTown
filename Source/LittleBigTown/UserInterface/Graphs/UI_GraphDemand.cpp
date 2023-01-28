// Fill out your copyright notice in the Description page of Project Settings.


#include "LittleBigTown/UserInterface/Graphs/UI_GraphDemand.h"
#include "LittleBigTown/GameSystem/MainGameState.h"
#include "LittleBigTown/Core/Debugger.h"

#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Components/HorizontalBox.h"

void UUI_GraphDemand::Display(TEnumAsByte<EWealthLevels> WealthLevel)
{
	LastWealthLevel = WealthLevel;
	UpdateFrequencies();
	GameState->OnNewHourDelegate.AddDynamic(this, &ThisClass::UpdateNewHour);

	SetVisibility(ESlateVisibility::Visible);	
}

void UUI_GraphDemand::NativeConstruct()
{
	Super::NativeConstruct();

	GameState = Cast <AMainGameState> (UGameplayStatics::GetGameState(GetWorld()));
	Button_Poor->OnClicked.AddDynamic(this, &ThisClass::PoorButtonClicked);
	Button_Middle->OnClicked.AddDynamic(this, &ThisClass::MiddleButtonClicked);
	Button_Rich->OnClicked.AddDynamic(this, &ThisClass::RichButtonClicked);
}

void UUI_GraphDemand::Hide()
{
	GameState->OnNewHourDelegate.RemoveDynamic(this, &ThisClass::UpdateNewHour);
	SetVisibility(ESlateVisibility::Collapsed);
}

void UUI_GraphDemand::UpdateFrequencies()
{
	const auto Frequencies { GameState->GetSpecialtiesFrequencies(LastWealthLevel) };
	auto PB_Table { HB_ProgressBars->GetAllChildren() };

	// ProgressBars configuration
	for (int i{ 0 }; i < Frequencies.Num(); ++i)
	{
		auto PB { Cast <UProgressBar> (PB_Table[i]) };
		ReverseProgressBar(Frequencies[i] < 0, PB);
		PB->SetPercent(Frequencies[i] >= 0 ? Frequencies[i] : Frequencies[i] * -1.0);
	}
}

void UUI_GraphDemand::UpdateNewHour(int32 NewHour)
{
	UpdateFrequencies();
}

void UUI_GraphDemand::ReverseProgressBar(bool Reverse, UProgressBar* ProgressBar)
{

#ifdef DEBUG_ONLY

	checkf(ProgressBar, TEXT("Error in UUI_GraphDemand::ReverseProgressBar, ProgressBar == nullptr."));

#endif

	if (!ProgressBar)
		return;

	if (Reverse)
	{
		ProgressBar->BarFillType = EProgressBarFillType::TopToBottom;
		ProgressBar->RenderTransform.Translation.Y = 202.5;
	}
	else if (!Reverse)
	{
		ProgressBar->BarFillType = EProgressBarFillType::BottomToTop;
		ProgressBar->RenderTransform.Translation.Y = 0.0;
	}

	ProgressBar->SynchronizeProperties();
}

void UUI_GraphDemand::PoorButtonClicked()
{
	LastWealthLevel = EWealthLevels::Poor;
	UpdateFrequencies();
}

void UUI_GraphDemand::MiddleButtonClicked()
{
	LastWealthLevel = EWealthLevels::Middle;
	UpdateFrequencies();
}

void UUI_GraphDemand::RichButtonClicked()
{
	LastWealthLevel = EWealthLevels::Rich;
	UpdateFrequencies();
}
