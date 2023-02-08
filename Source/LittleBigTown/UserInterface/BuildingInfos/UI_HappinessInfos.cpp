// Fill out your copyright notice in the Description page of Project Settings.


#include "LittleBigTown/UserInterface/BuildingInfos/UI_HappinessInfos.h"
#include "LittleBigTown/UserInterface/BuildingInfos/UI_ItemHappinessInfos.h"
#include "LittleBigTown/Core/Enums.h"
#include "LittleBigTown/Core/Debugger.h"
#include "LittleBigTown/Actors/ResidentialBuilding.h"
#include "LittleBigTown/GameSystem/MainGameState.h"
#include "Components/VerticalBox.h"
#include "LittleBigTown/UserInterface/Templates/BGButton.h"
#include "Components/VerticalBoxSlot.h"
#include "Kismet/GameplayStatics.h"
#include "Algo/Count.h"


#define BUTTON_SIZE 35.0f
#define DEFAULT_PADDING 4.0f
#define SUB_WIDGET_ITEM_SIZE 25.0f * SubClassWidget->GetChildAt(0)->RenderTransform.Scale.Y


void UUI_HappinessInfos::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UUI_HappinessInfos::NativeConstruct()
{
	Super::NativeConstruct();

	for (const auto Widget : VB_Main->GetAllChildren())
	{
		const auto Button { Cast <UBGButton>(Widget) };
		Button->OnBGButtonClicked.AddDynamic(this, &ThisClass::OnItemClicked);
		ArrItems.Add(Cast <UUI_ItemHappinessInfos>(Cast <UContentWidget>(Button->GetButton())->GetChildAt(0)));
	}

	MainGameState = Cast <AMainGameState>(UGameplayStatics::GetGameState(GetWorld()));

#ifdef DEBUG_ONLY

	checkf(ArrItems.Num() == 7, TEXT("Error in UUI_HappinessInfos::NativeConstruct : ArrButtons.Num() != 7."));
	checkf(SubClassWidget->GetAllChildren().Num() == 7, 
		TEXT("Error in UUI_HappinessInfos::NativeConstruct : Children of SubClassWidget != 7."));

#endif
}

void UUI_HappinessInfos::DEBUGPrintClasses()
{
	TArray <FString> Names{ "Industrie", "Finance", "Science", "Tourism", "Crime", "Military", "Spiritual" };

	for (int32 i{ 0 }; i < Names.Num(); ++i)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red,
			FString::Printf(TEXT("%s : %d"), *Names[i], GetSpeArray(ECitySpecialty(i)).Num()));
		DEBUGPrintSubClasses(ECitySpecialty(i));
	}
}

void UUI_HappinessInfos::DEBUGPrintSubClasses(const ECitySpecialty& Specialty)
{
	TMap <FName, int32> Count;
	TMap <FName, float> Mean;

	// Init Map
	for (const auto& Pair : MainGameState->GetSocialClasses(BuildingWealth).GetMap(Specialty))
	{
		Count.Add(Pair.Key, 0);
		Mean.Add(Pair.Key, 0);
	}

	for (const auto& Resident : GetSpeArray(Specialty))
	{
		++Count[Resident.m_SubClassName];
		Mean[Resident.m_SubClassName] += Resident.m_Satisfaction;
	}

	for (const auto & Pair : Count)
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, 
			FString::Printf(TEXT("%s : Count : %d, Mean : %f"), *Pair.Key.ToString(), Pair.Value, Mean[Pair.Key] / Pair.Value));
}

TArray<struct FResident>& UUI_HappinessInfos::GetSpeArray(const TEnumAsByte<enum ECitySpecialty>& Specialty)
{
#ifdef DEBUG_ONLY

	checkf(Specialty != DefaultCitySpecialtyEnum,
		TEXT("Error in UUI_HappinessInfos::GetSortedArray, Specialty is Default."));

#endif

	switch (Specialty)
	{
	case Industry: return m_Industry; break;
	case Finance: return m_Finance; break;
	case Science: return m_Science; break;
	case Tourism: return m_Tourism; break;
	case Crime: return m_Crime; break;
	case Military: return m_Military; break;
	case Spiritual: return m_Spiritual; break;
	default: return m_Industry; break;
	}
}

UUI_ItemHappinessInfos* UUI_HappinessInfos::GetSpeItem(const TEnumAsByte<enum ECitySpecialty>& Specialty)
{
	return ArrItems[int32(Specialty)];
}

void UUI_HappinessInfos::EmptySpeArrays()
{
	for (int32 i{ 0 }; i < int32(DefaultCitySpecialtyEnum); ++i)
		GetSpeArray(ECitySpecialty(i)).Empty();
}

void UUI_HappinessInfos::ResetSubClassWidget()
{
	for (const auto SubWidget : SubClassWidget->GetAllChildren())
		SubWidget->SetVisibility(ESlateVisibility::Collapsed);

	SubClassWidget->RenderTransform.Translation.Y = 0.0f;
	SubClassWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void UUI_HappinessInfos::MainDisplay(AResidentialBuilding* Building, bool NewDisplay)
{
	TMap <ECitySpecialty, float> Satisfaction { {Industry, 0.0f}, {Finance, 0.0f}, {Science, 0.0f}, {Tourism, 0.0f}, 
		{Crime, 0.0f}, {Military, 0.0f}, {Spiritual, 0.0f} };

	EmptySpeArrays();

	if (NewDisplay)
	{
		BuildingWealth = Building->GetInfosBase().m_WealthLevel;
		ResetWidget(ESlateVisibility::Collapsed);
	}

	for (const auto& Resident : Building->GetResidents())
	{

#ifdef DEBUG_ONLY

		checkf(Resident.m_Type != DefaultCitySpecialtyEnum, 
			TEXT("Error in UUI_HappinessInfos::SortBuildingArray, Resident Type in Building Array is Default."));

#endif
		GetSpeArray(Resident.m_Type).Add(Resident);
		Satisfaction[Resident.m_Type] += Resident.m_Satisfaction;
	}

	for (const auto Element : Satisfaction)
	{
		GetSpeItem(Element.Key)->SetInformations(GetSpeArray(Element.Key).Num(), Element.Value / GetSpeArray(Element.Key).Num());

		// GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("%s"), GetSpeArray(Element.Key).Num() == 0 ? TEXT("true") : TEXT("false")));
		if (NewDisplay)
			GetSpeItem(Element.Key)->SetSpecialtyName(MainGameState->GetSpecialtyNames(Building->GetInfosBase().m_WealthLevel)[int32(Element.Key)]);
	}

	if (SubClassWidget->GetVisibility() == ESlateVisibility::SelfHitTestInvisible && LastButtonClicked.IsValid())
		SubClassDisplay(LastButtonClicked.Get(), false);

	// DEBUGPrintClasses();
}
void UUI_HappinessInfos::ResetWidget(ESlateVisibility Visible)
{
	if (Visible != ESlateVisibility::Collapsed && Visible != ESlateVisibility::Hidden)
		return;

	if (LastButtonClicked.IsValid())
	{
		Cast <UVerticalBoxSlot> (LastButtonClicked.Get()->Slot)->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 0.0f));
		ResetLastButtonClicked();
	}

	ResetSubClassWidget();
}

void UUI_HappinessInfos::OnItemClicked(UBGButton* Button)
{
	// Pas d'interaction avec le Button si le Count de son Item vaut 0
	if (Cast <UUI_ItemHappinessInfos>(Button->GetButton()->GetChildAt(0))->GetCount() == 0)
	{
		Button->SetButtonClicked(false);
		return;
	}


	auto const ResetButton { ResetLastButtonClicked() };

	// Un Button a été reset :
	if (ResetButton)
	{
		Cast <UVerticalBoxSlot>(ResetButton->Slot)->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 0.0f));
		ResetSubClassWidget();
	}

	// Si le Button Clicked n'est pas le même que le Button Reset
	if (Button != ResetButton)
	{
		LastButtonClicked = Button;
		SubClassDisplay(Button, true);
	}
}

void UUI_HappinessInfos::SubClassDisplay(UBGButton* Button, bool NewDisplay)
{
	int32 Index {VB_Main->GetChildIndex(Button)};
	int32 i{ 0 };
	TMap <FName, float> Count {};

	// Init Map
	for (const auto& Pair : MainGameState->GetSocialClasses(BuildingWealth).GetMap(ECitySpecialty(Index)))
		Count.Add(Pair.Key, 0.0f);

	TMap <FName, float> Mean { Count };

	for (const auto& Element : GetSpeArray(ECitySpecialty(Index)))
	{
		Mean[Element.m_SubClassName] += Element.m_Satisfaction;
		++Count[Element.m_SubClassName];
	}

	for (const auto& Pair : Mean)
	{
		// int32 Count{ int32(Algo::CountIf(GetSpeArray(ECitySpecialty(Index)), [&Pair](const FResident& Resident)-> bool
			// { return Resident.m_SubClassName == Pair.Key; })) };

		const auto Item { Cast <UUI_ItemHappinessInfos> (SubClassWidget->GetAllChildren()[i]) };
		Item->SetInformations(Count[Pair.Key], Pair.Value / Count[Pair.Key]);

		if (NewDisplay)
		{
			Item->SetSpecialtyName(FText::FromString(Pair.Key.ToString()));
			Item->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		++i;
	}

	SubClassWidget->RenderTransform.Translation.Y = BUTTON_SIZE * (Index - 6) + DEFAULT_PADDING;
	SubClassWidget->SetRenderTransform(SubClassWidget->RenderTransform);
	SubClassWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	Cast <UVerticalBoxSlot>(Button->Slot)->SetPadding(FMargin(0.0f, 0.0f, 0.0f, (Count.Num() * SUB_WIDGET_ITEM_SIZE) + DEFAULT_PADDING * 2));
}

