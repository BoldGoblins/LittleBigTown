// Fill out your copyright notice in the Description page of Project Settings.


#include "LittleBigTown/UserInterface/BuildingInfos/UI_NeedsInfos.h"

#include "LittleBigTown/UserInterface/Templates/Components/BGComboBoxString.h"
#include "LittleBigTown/UserInterface/BuildingInfos/UI_BuildingInfos.h"
#include "Components/GridPanel.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/ProgressBar.h"
#include "Components/ComboBoxString.h"
#include "LittleBigTown/GameSystem/MainGameState.h"
#include "LittleBigTown/GameSystem/BGMainPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "LittleBigTown/Core/Debugger.h"

#define COMBOBOX_INDEX 1

void UUI_NeedsInfos::NativeConstruct()
{
	Super::NativeConstruct();

	MainGameState = Cast <AMainGameState> (UGameplayStatics::GetGameState(GetWorld()));
	MainPlayerController = Cast <ABGMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	CB_Classes = Cast <UBGComboBoxString>(GP_Classes->GetChildAt(COMBOBOX_INDEX));
	CB_SubClasses = Cast <UBGComboBoxString>(GP_SubClasses->GetChildAt(COMBOBOX_INDEX));

	CB_Classes->OnBGComboBoxStringChanged.AddDynamic(this, &ThisClass::ComboSelectionChanged);
	CB_SubClasses->OnBGComboBoxStringChanged.AddDynamic(this, &ThisClass::ComboSelectionChanged);

#ifdef DEBUG_ONLY

	checkf(IsValid(MainPlayerController), TEXT("Error in UUI_NeedsInfos::NativeConstruct, PlayerController Ref Invalid."));

#endif
}

void UUI_NeedsInfos::SetInformations(ABuilding* Building, bool bNewDisplay)
{
	if (bNewDisplay)
	{
		SubClassesOptions.Empty();

		CB_Classes->Populate(MainGameState->GetSpecialtyNames(Building->GetInfosBase().m_WealthLevel), true);

		for (ECitySpecialty Spe : TEnumRange<ECitySpecialty>())
			for (const auto& Pair : MainGameState->GetSocialClasses(Building->GetInfosBase().m_WealthLevel).GetMap(Spe))
				SubClassesOptions.Add(Pair.Key);

		CB_SubClasses->Populate(SubClassesOptions, true);
	}

	if (Building->IsA <AResidentialBuilding>())
		DisplayInformations(Cast <AResidentialBuilding>(Building));
}

void UUI_NeedsInfos::Reset()
{
}

void UUI_NeedsInfos::ComboSelectionChanged(UBGComboBoxString* ComboBox, const FString& Option, int32 Index)
{
	if (!IsValid(MainPlayerController))
		return;

	const auto& Building { MainPlayerController->GetBuildingInfos()->GetBuildingDisplayed() };

	if (!Building.IsValid())
		return;

	if (CB_Classes == ComboBox)
	{
		if (Index == 0)
			CB_SubClasses->Populate(SubClassesOptions, true);

		else
		{
			TArray <FName> Names{};

			MainGameState->GetSocialClasses(Building->GetInfosBase().m_WealthLevel).GetMap(ECitySpecialty(Index - 1)).GetKeys(Names);
			CB_SubClasses->Populate(Names, true);
		}
		// Cas où la Box Classes est modifiée
		if (Building->IsA <AResidentialBuilding>())
			PopulateScrollAndCount(Index == 0 ? ComputeInfos(Building->GetAllOccupants()) : 
				ComputeInfos(Building->GetOccupants(ECitySpecialty(Index - 1))));
		// else : Populate pour les Commerces
	}
	else if (CB_SubClasses == ComboBox)
	{
		// Cas où la Box SubClasse est modifiée
		if (Building->IsA <AResidentialBuilding>())
			DisplayInformations(Cast <AResidentialBuilding>(Building));
		// else : Populate pour les Commerces
	}
}

void UUI_NeedsInfos::DisplayInformations(const AResidentialBuilding* Building)
{
	const UComboBoxString* CB { Cast <UComboBoxString>(CB_SubClasses->GetWrappedWidget()) };

	if (CB->GetSelectedIndex() != 0)
	{
		PopulateScrollAndCount(ComputeInfos(Building->GetOccupants(CB->GetSelectedOption())));

		return;
	}

	CB = Cast <UComboBoxString>(CB_Classes->GetWrappedWidget());

	if (CB->GetSelectedIndex() == 0)
		PopulateScrollAndCount(ComputeInfos(Building->GetAllOccupants()));
	else
		PopulateScrollAndCount(ComputeInfos(Building->GetOccupants(ECitySpecialty(CB->GetSelectedIndex() - 1))));
}

void UUI_NeedsInfos::PopulateScrollAndCount(const TTuple <FNeeds, int32>& Infos)
{
	// GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("Needs : %f"), Needs.m_Culture));
	TB_Count->SetText(FText::FromString(FString::FromInt(Infos.Value)));

	Cast <UProgressBar>(VB_ProgressBars->GetChildAt(0))->SetPercent(Infos.Key.m_FoodAndDrinks);
	Cast <UProgressBar>(VB_ProgressBars->GetChildAt(1))->SetPercent(Infos.Key.m_Retail);
	Cast <UProgressBar>(VB_ProgressBars->GetChildAt(2))->SetPercent(Infos.Key.m_Health);
	Cast <UProgressBar>(VB_ProgressBars->GetChildAt(3))->SetPercent(Infos.Key.m_Entertainment);
	Cast <UProgressBar>(VB_ProgressBars->GetChildAt(4))->SetPercent(Infos.Key.m_Social);
	Cast <UProgressBar>(VB_ProgressBars->GetChildAt(5))->SetPercent(Infos.Key.m_Luxury);
	Cast <UProgressBar>(VB_ProgressBars->GetChildAt(6))->SetPercent(Infos.Key.m_Culture);
	Cast <UProgressBar>(VB_ProgressBars->GetChildAt(7))->SetPercent(Infos.Key.m_Tourism);
}

TTuple <FNeeds, int32> UUI_NeedsInfos::ComputeInfos(const TArray<FResident>& Arr)
{
	FNeeds Mean {};

	for (const auto& Resident : Arr)
		Mean += Resident.m_Needs;

	return MakeTuple(Mean / Arr.Num(), Arr.Num());
}
