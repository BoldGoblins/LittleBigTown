// Fill out your copyright notice in the Description page of Project Settings.


#include "LittleBigTown/UserInterface/BuildingInfos/UI_HappinessInfos.h"
#include "LittleBigTown/UserInterface/BuildingInfos/UI_ItemHappinessInfos.h"
#include "LittleBigTown/UserInterface/BuildingInfos/UI_BuildingInfos.h"
#include "LittleBigTown/Core/Enums.h"
#include "LittleBigTown/Actors/ResidentialBuilding.h"
#include "LittleBigTown/GameSystem/MainGameState.h"
#include "LittleBigTown/GameSystem/BGMainPlayerController.h"
#include "Components/VerticalBox.h"
#include "LittleBigTown/UserInterface/Templates/Components/BGButton.h"
#include "Components/VerticalBoxSlot.h"
#include "Kismet/GameplayStatics.h"
#include "LittleBigTown/Core/Debugger.h"


#define BUTTON_SIZE 35.0f
#define DEFAULT_PADDING 4.0f
#define SUB_WIDGET_ITEM_SIZE 25.0f * SubClassWidget->GetChildAt(0)->RenderTransform.Scale.Y

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
	MainPlayerController = Cast <ABGMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

#ifdef DEBUG_ONLY

	checkf(ArrItems.Num() == 7, TEXT("Error in UUI_HappinessInfos::NativeConstruct : ArrButtons.Num() != 7."));
	checkf(SubClassWidget->GetAllChildren().Num() == 7, 
		TEXT("Error in UUI_HappinessInfos::NativeConstruct : Children of SubClassWidget != 7."));

	checkf(IsValid(MainPlayerController), TEXT("Error in UUI_HappinessInfos::NativeConstruct, PlayerController Ref invalid."));

#endif
}

UUI_ItemHappinessInfos* UUI_HappinessInfos::GetSpeItem(const TEnumAsByte<enum ECitySpecialty>& Specialty)
{
	return ArrItems[uint8(Specialty.GetValue())];
}
bool UUI_HappinessInfos::IsSubClassDisplayed() const
{
	return SubClassWidget->GetVisibility() == ESlateVisibility::SelfHitTestInvisible && LastButtonClicked.IsValid();
}

void UUI_HappinessInfos::ResetSubClassesWidget()
{
	for (const auto SubWidget : SubClassWidget->GetAllChildren())
		SubWidget->SetVisibility(ESlateVisibility::Collapsed);

	SubClassWidget->RenderTransform.Translation.Y = 0.0f;
	SubClassWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void UUI_HappinessInfos::SetClassesInformations(AResidentialBuilding* ResBuilding, bool bNewDisplay)
{
	TMap <ECitySpecialty, float> Satisfaction{};

	if (bNewDisplay)
		Reset();

	for (const auto Spe : TEnumRange<ECitySpecialty>())
	{
		Satisfaction.Add(Spe, 0.0f);

		for (const auto& Resident : ResBuilding->GetOccupants(Spe))
			Satisfaction[Resident.m_Type] += Resident.m_Satisfaction;
		// En passant Resident.m_Type ici à la place de Spé, on s'assure que les Résidents dans la map sont tous du bon type.
	}

	for (const auto& Element : Satisfaction)
	{
		GetSpeItem(Element.Key)->PopulateInformations(ResBuilding->GetOccupants(Element.Key).Num(), 
			Element.Value / ResBuilding->GetOccupants(Element.Key).Num());

		// GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("%s"), GetSpeArray(Element.Key).Num() == 0 ? TEXT("true") : TEXT("false")));
		if (bNewDisplay)
			GetSpeItem(Element.Key)->PopulateName(MainGameState
				->GetSpecialtyNames(ResBuilding->GetInfosBase().m_WealthLevel)[int32(Element.Key)]);
	}
}

void UUI_HappinessInfos::SetInformations(ABuilding* Building, bool bNewDisplay)
{
	if (Building->IsA <AResidentialBuilding>())
	{
		SetClassesInformations(Cast <AResidentialBuilding>(Building), bNewDisplay);

		if (IsSubClassDisplayed())
			SetSubClassesInformations(LastButtonClicked.Get(), Cast <AResidentialBuilding>(Building), false);
	}
}

void UUI_HappinessInfos::Reset()
{
	if (LastButtonClicked.IsValid())
	{
		Cast <UVerticalBoxSlot>(LastButtonClicked.Get()->Slot)->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 0.0f));
		ResetLastButtonClicked();
	}

	ResetSubClassesWidget();
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
		ResetSubClassesWidget();
	}

	// Si le Button Clicked n'est pas le même que le Button Reset
	if (Button != ResetButton && IsValid(MainPlayerController))
	{
		const auto& BuildingPtr { MainPlayerController->GetBuildingInfos()->GetBuildingDisplayed() };
		LastButtonClicked = Button;

		if (BuildingPtr.IsValid() && BuildingPtr.Get()->IsA<AResidentialBuilding>())
			SetSubClassesInformations(Button, Cast<AResidentialBuilding>(BuildingPtr), true);
	}
}

void UUI_HappinessInfos::SetSubClassesInformations(class UBGButton* Button, class AResidentialBuilding* ResBuilding, bool NewDisplay)
{
	uint8 Index { uint8(VB_Main->GetChildIndex(Button)) };
	int32 i{ 0 };
	TMap <FName, float> Count{};

	// Init Map
	for (const auto& Pair : MainGameState->GetSocialClasses(ResBuilding->GetInfosBase().m_WealthLevel).GetMap(ECitySpecialty(Index)))
		Count.Add(Pair.Key, 0.0f);

	TMap <FName, float> Mean { Count };

	for (const auto& Element : ResBuilding->GetOccupants(ECitySpecialty(Index)))
	{
		Mean[Element.m_SubClassName] += Element.m_Satisfaction;
		++Count[Element.m_SubClassName];
	}

	for (const auto& Pair : Mean)
	{
		const auto Item{ Cast <UUI_ItemHappinessInfos>(SubClassWidget->GetAllChildren()[i]) };
		Item->PopulateInformations(Count[Pair.Key], Pair.Value / Count[Pair.Key]);

		if (NewDisplay)
		{
			Item->PopulateName(FText::FromString(Pair.Key.ToString()));
			Item->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		++i;
	}

	SubClassWidget->RenderTransform.Translation.Y = BUTTON_SIZE * (Index - 6) + DEFAULT_PADDING;
	SubClassWidget->SetRenderTransform(SubClassWidget->RenderTransform);
	SubClassWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	Cast <UVerticalBoxSlot>(Button->Slot)->SetPadding(FMargin(0.0f, 0.0f, 0.0f, (Count.Num() * SUB_WIDGET_ITEM_SIZE) + DEFAULT_PADDING * 2));
}

