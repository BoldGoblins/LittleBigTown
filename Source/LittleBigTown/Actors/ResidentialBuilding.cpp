// Fill out your copyright notice in the Description page of Project Settings.


#include "ResidentialBuilding.h"
#include "LittleBigTown/UserInterface/BuildingInfos/UI_BuildingInfos.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "LittleBigTown/GameSystem/MainGameState.h"
#include "LittleBigTown/Core/Debugger.h"
#include "LittleBigTown/Core/SocialClass.h"

void AResidentialBuilding::BeginPlay()
{
	Super::BeginPlay();

	MainGameState = Cast <AMainGameState>(UGameplayStatics::GetGameState(GetWorld()));

	// Set Infos values (when EditDefaultsOnly values are set in Editor)
	// InfosBase.m_MaxLevel = MAX_LEVEL_RESIDENTIAL;
	// ResidentialInformations.m_TotalIncomes = IncomePerHabitant * InfosBase.m_OccupationCurrentCount * CurrentLevel;

	MainGameState->OnNewHourDelegate.AddDynamic(this, &ThisClass::UpdateNewHour);

}

void AResidentialBuilding::GenerateResidents(int32 Count)
{

#ifdef DEBUG_ONLY

	checkf(Count > 0, TEXT("Error in AResidentialBuilding::GenerateResidents : Count > 0."));

#endif 

	const auto Frequencies { MainGameState->GetSpecialtiesFrequencies(InfosBase.m_WealthLevel) };
	TEnumAsByte <ECitySpecialty> Specialty{ DefaultCitySpecialtyEnum };
	TArray<int> Arr{};
	Arr.Init(0, Frequencies.Num());
	auto It { Frequencies.CreateConstIterator()};
	Arr[0] = FMath::RoundToInt(It->Value * 100) + 100;

	for (int i{ 1 }; i < Arr.Num(); ++i)
		Arr[i] = (FMath::RoundToInt((++It)->Value * 100) + 100) + Arr[i -1];

	for (int i{ 0 }; i < Count; ++i)
	{
		int32 x { FMath::RandRange(1, Arr[Arr.Num() - 1])};

		// Cannot res It
		auto NewIt{ Frequencies.CreateConstIterator() };

		if (x > 0 && x <= Arr[0])
			Specialty = NewIt->Key;
		else
		{
			for (int j{ 1 }; j < Arr.Num(); ++j)
			{
				++NewIt;

				if(x > Arr[j - 1] && x <= Arr[j])
					Specialty = NewIt->Key;
			}
		}	
		m_Residents.Add(FResident(Specialty, MainGameState->GetSocialClasses(InfosBase.m_WealthLevel)));
		MainGameState->AddOrSubResidents(InfosBase.m_WealthLevel, Specialty, 1, ResidentialInformations.m_IncomesPerHab);
	}
}

void AResidentialBuilding::UpdateNewHour(int32 Hour)
{
	double Demand { MainGameState->GetResidentialDemand(InfosBase.m_WealthLevel) };

	int32 OccupationVar { FMath::RoundToInt((Demand * TEN_PERCENT_OCCUPATION) + 
		((ResidentialInformations.m_SatisfactionPercent - 0.5) * TEN_PERCENT_OCCUPATION)) };

	OccupationVar = FMath::Clamp(OccupationVar, -InfosBase.m_OccupationCurrentCount, InfosBase.m_OccupationMaxCount - InfosBase.m_OccupationCurrentCount);
	/*
	if (InfosBase.m_OccupationCurrentCount + OccupationVar > InfosBase.m_OccupationMaxCount)
		OccupationVar = InfosBase.m_OccupationMaxCount - InfosBase.m_OccupationCurrentCount;

	else if (InfosBase.m_OccupationCurrentCount + OccupationVar < 0)
		OccupationVar = -InfosBase.m_OccupationCurrentCount;
*/
	// Modification des infos de l'habitation :
	InfosBase.m_OccupationCurrentCount += OccupationVar;
	ResidentialInformations.m_TotalIncomes += OccupationVar * ResidentialInformations.m_IncomesPerHab;

	if (OccupationVar > 0)
		GenerateResidents(OccupationVar);

	else if (OccupationVar < 0)
	{
		// méthode pour retirer habitants

		for (int i{ 0 }; i < OccupationVar * (-1); ++i)
		{
			MainGameState->AddOrSubResidents(InfosBase.m_WealthLevel, m_Residents[0].GetType(), -1, ResidentialInformations.m_IncomesPerHab);
			m_Residents.RemoveAt(0);
			// GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("%d"), m_Residents.Num()));
		}
	}

	else
		return;

	OnResBuildingInfosChangedDelegate.Broadcast();
}

void AResidentialBuilding::AddSatisfaction(float Satisfaction)
{
/*
#ifdef DEBUG_ONLY

	checkf(Satisfaction >= -1.0f && Satisfaction <= 1.0f, 
		TEXT("Error in AResidentialBuilding::SetSatisfaction : Satisfaction has'nt a valid value."));

#endif 
*/

	ResidentialInformations.m_SatisfactionPercent = FMath::Clamp(ResidentialInformations.m_SatisfactionPercent + Satisfaction, 0.0f, 1.0f);

	// Declare Building Infos Modification :
	OnResBuildingInfosChangedDelegate.Broadcast();
}
