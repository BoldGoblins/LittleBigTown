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

	for (const auto element : HB_ProgressBars->GetAllChildren())
		element->SetVisibility(ESlateVisibility::Hidden);

	for (const auto element : Frequencies)
	{
		ReverseProgressBar(element.Value < 0.0f, GetProgressBar(element.Key));
		GetProgressBar(element.Key)->SetPercent(element.Value >= 0.0f ? element.Value : element.Value * -1.0);
		GetProgressBar(element.Key)->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
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

UProgressBar* UUI_GraphDemand::GetProgressBar(const TEnumAsByte<enum ECitySpecialty>& Specialty) const
{

#ifdef DEBUG_ONLY

	checkf(Specialty != ECitySpecialty::DefaultCitySpecialtyEnum, TEXT("Error in UUI_GraphDemand::GetFromSpecialty, Specialty == Default."));

#endif

	switch (Specialty)
	{
	case ECitySpecialty::Industry: return PB_Industry; break;
	case ECitySpecialty::Finance: return PB_Finance; break;
	case ECitySpecialty::Science: return PB_Science; break;
	case ECitySpecialty::Tourism: return PB_Tourism; break;
	case ECitySpecialty::Crime: return PB_Crime; break;
	case ECitySpecialty::Military: return PB_Military; break;
	case ECitySpecialty::Spiritual: return PB_Spiritual; break;
	default: return PB_Industry; break;
	}
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
