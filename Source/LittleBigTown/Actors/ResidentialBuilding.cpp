// Fill out your copyright notice in the Description page of Project Settings.


#include "ResidentialBuilding.h"
#include "LittleBigTown/UserInterface/BuildingInfos/UI_BuildingInfos.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "LittleBigTown/GameSystem/MainGameState.h"
#include "LittleBigTown/Core/Debugger.h"


void AResidentialBuilding::BeginPlay()
{
	Super::BeginPlay();

	MainGameState = Cast <AMainGameState>(UGameplayStatics::GetGameState(GetWorld()));

	// Set Infos values (when EditDefaultsOnly values are set in Editor)
	InfosBase.m_MaxLevel = MAX_LEVEL_RESIDENTIAL;
	InfosBase.m_OccupationMaxCount = ResidentsMaxCount;;
	ResidentialInformations.m_SatisfactionPercent = BASE_SATISFACTION;
	ResidentialInformations.m_IncomesPerHab = IncomePerHabitant;
	ResidentialInformations.m_TotalIncomes = IncomePerHabitant * InfosBase.m_OccupationCurrentCount * CurrentLevel;

	MainGameState->OnNewHourDelegate.AddDynamic(this, &ThisClass::UpdateNewHour);

}

void AResidentialBuilding::GenerateResidents(int32 Count)
{
	const auto Frequencies { MainGameState->GetSpecialtiesFrequencies(InfosBase.m_WealthLevel) };

	TArray<int> Arr{};
	Arr.Init(0, Frequencies.Num());

	Arr[0] = FMath::RoundToInt(Frequencies[0] * 100) + 100;

	for (int i{ 1 }; i < Arr.Num(); ++i)
		Arr[i] = (FMath::RoundToInt(Frequencies[1] * 100) + 100) + Arr[i -1];

	for (int i{ 0 }; i < Count; ++i)
	{
		int32 x { FMath::RandRange(1, Arr[6]) };

		auto Specialty {ECitySpecialty::DefaultCitySpecialtyEnum};
		
		if (x > 0 && x <= Arr[0]) Specialty = ECitySpecialty::Industry;
		else if (x > Arr[0] && x <= Arr[1]) Specialty = ECitySpecialty::Finance;
		else if (x > Arr[1] && x <= Arr[2]) Specialty = ECitySpecialty::Science;
		else if (x > Arr[2] && x <= Arr[3]) Specialty = ECitySpecialty::Tourism;
		else if (x > Arr[3] && x <= Arr[4]) Specialty = ECitySpecialty::Crime;
		else if (x > Arr[4] && x <= Arr[5]) Specialty = ECitySpecialty::Military;
		else if (x > Arr[5] && x <= Arr[6]) Specialty = ECitySpecialty::Spiritual;

		m_Residents.Add(Resident(Specialty));
		MainGameState->AddOrSubResidents(InfosBase.m_WealthLevel, Specialty, 1, ResidentialInformations.m_IncomesPerHab);
	}
}

void AResidentialBuilding::UpdateNewHour(int32 Hour)
{
	double Demand { MainGameState->GetResidentialDemand(InfosBase.m_WealthLevel) };

	int32 OccupationVar { FMath::RoundToInt((Demand * TEN_PERCENT_OCCUPATION) + 
		((ResidentialInformations.m_SatisfactionPercent - 0.5) * TEN_PERCENT_OCCUPATION)) };
	
	if (InfosBase.m_OccupationCurrentCount + OccupationVar > InfosBase.m_OccupationMaxCount)
		OccupationVar = InfosBase.m_OccupationMaxCount - InfosBase.m_OccupationCurrentCount;

	else if (InfosBase.m_OccupationCurrentCount + OccupationVar < 0)
		OccupationVar = -InfosBase.m_OccupationCurrentCount;

	InfosBase.m_OccupationCurrentCount += OccupationVar;

	ResidentialInformations.m_TotalIncomes += OccupationVar * ResidentialInformations.m_IncomesPerHab;
	// MainGameState->AddOrSubResidents(InfosBase.m_WealthLevel, ECitySpecialty::Industry, OccupationVar, ResidentialInformations.m_IncomesPerHab);
	if(OccupationVar != 0 )
		GenerateResidents(OccupationVar);

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
