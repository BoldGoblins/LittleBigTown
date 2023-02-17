// Fill out your copyright notice in the Description page of Project Settings.


#include "ResidentialBuilding.h"
#include "LittleBigTown/UserInterface/BuildingInfos/UI_BuildingInfos.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "LittleBigTown/GameSystem/MainGameState.h"
#include "LittleBigTown/Core/Debugger.h"
#include "LittleBigTown/Core/SocialClass.h"
#include "Algo/Sort.h"

#define MAX_LEVEL_RESIDENTIAL 5
// #define TEN_PERCENT_OCCUPATION FMath::RoundToInt ((InfosBase.m_OccupationMaxCount * 10) / 100)

void AResidentialBuilding::BeginPlay()
{
	Super::BeginPlay();

	MainGameState = Cast <AMainGameState>(UGameplayStatics::GetGameState(GetWorld()));
	MainGameState->OnNewHourDelegate.AddDynamic(this, &ThisClass::UpdateNewHour);


#ifdef DEBUG_ONLY

	checkf(InfosBase.m_OccupationMaxCount >= 5, TEXT("Error in AResidentialBuilding::BeginPlay : Minimum d'habitants requis = 5."));

#endif 

}

void AResidentialBuilding::GenerateResidents(int32 Count)
{

#ifdef DEBUG_ONLY

	checkf(Count > 0, TEXT("Error in AResidentialBuilding::GenerateResidents : Count > 0."));

#endif 

	const auto Frequencies { MainGameState->GetSpecialtiesFrequencies(InfosBase.m_WealthLevel) };
	TEnumAsByte <ECitySpecialty> Specialty{ ECitySpecialty::DefaultCitySpecialtyEnum };
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

		m_Occupants.Add(FResident(Specialty, MainGameState->GetSocialClasses(InfosBase.m_WealthLevel)));
		MainGameState->AddOrSubResidents(InfosBase.m_WealthLevel, Specialty, 1, ResidentialInformations.m_IncomesPerHab);
	}
}

void AResidentialBuilding::RemoveResidents(int32 Count)
{
#ifdef DEBUG_ONLY

	checkf(Count < 0, TEXT("Error in AResidentialBuilding::RemoveResidents : Count < 0."));

#endif 

	// Sort by Satisfaction : Lowers are at Lowers Index
	Algo::Sort(m_Occupants, [](const FResident& Curr, const FResident& Next) -> bool {
		return Curr.m_Satisfaction < Next.m_Satisfaction; });

	for (int32 i { 0 }; i  < Count * (-1); i ++)
		MainGameState->AddOrSubResidents(InfosBase.m_WealthLevel, m_Occupants[0].m_Type, -1, ResidentialInformations.m_IncomesPerHab);

	m_Occupants.RemoveAt(0, Count * (-1), true);
}

void AResidentialBuilding::UpdateNewHour(int32 Hour)
{
	double Demand { MainGameState->GetResidentialDemand(InfosBase.m_WealthLevel) };
	const int32 Constant {FMath::Clamp(FMath::RoundToInt((InfosBase.m_OccupationMaxCount * 10) / 100), 1, 500)};
	int32 OccupationVar { FMath::RoundToInt((Demand * Constant) + ((ResidentialInformations.m_SatisfactionPercent - 0.5) * Constant)) };

	// Ne pas enlever plus d'habitants qu'il n'y en a ou ne pas en ajouter plus que le logement ne peut en contenir
	OccupationVar = FMath::Clamp(OccupationVar, -InfosBase.m_OccupationCurrentCount, InfosBase.m_OccupationMaxCount - InfosBase.m_OccupationCurrentCount);

	// Modification des infos de l'habitation :
	InfosBase.m_OccupationCurrentCount += OccupationVar;
	ResidentialInformations.m_TotalIncomes += OccupationVar * ResidentialInformations.m_IncomesPerHab;

	if (OccupationVar > 0)
		GenerateResidents(OccupationVar);

	else if (OccupationVar < 0)
		RemoveResidents(OccupationVar);

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
