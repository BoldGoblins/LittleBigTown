// Fill out your copyright notice in the Description page of Project Settings.


#include "LittleBigTown/GameSystem/City.h"
#include "Kismet/GameplayStatics.h"
#include "LittleBigTown/Actors/Building.h"
#include "LittleBigTown/Actors/ResidentialBuilding.h"
// #include "LittleBigTown/GameSystem/MainGameState.h"
// Sort
#include "Algo/Sort.h"
// debug
#include "LittleBigTown/Core/Debugger.h"

City::City()
{
	m_DemandPoor.Init(0.5, 7);
	m_DemandMiddle.Init(0.5, 7);
	m_DemandRich.Init(0.5, 7);

	// GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("%d"), m_DemandRich.Num()));
}
/*
void City::UpdateDemand(const TEnumAsByte<EWealthLevels>& WealthLevels, const TEnumAsByte<ECitySpecialty>& Specialty,  int32 Count)
{
	// int32 Index { int(Specialty) };
#ifdef DEBUG_ONLY

	checkf(WealthLevels != EWealthLevels::DefaultWealthEnum,
		TEXT("Error in City::UpdateDemand, WealthLevels == WealthLevels::DefaultWealthEnum."));

	// checkf(Index >= 0 && Index < 7, TEXT("Error in City::UpdateDemand, false conversion from Specialty to Index."));

	checkf(DemandModifier != 0, TEXT("Error in City::UpdateDemand, DemandModifier == 0. Check this value is set in MainGameState BeginPlay."));

#endif

	int32 Total { m_CityLevel * DemandModifier };
	float Percent { double(Count) / Total };

	switch (WealthLevels)
	{
	case EWealthLevels::Poor: m_Poor.AccessCategory(Specialty) = FMath::Clamp(m_Poor.AccessCategory(Specialty) - Percent, -1.0, 1.0); break;
	case EWealthLevels::Middle: m_Middle.AccessCategory(Specialty) = FMath::Clamp(m_Middle.AccessCategory(Specialty) - Percent, -1.0, 1.0); break;
	case EWealthLevels::Rich: m_Rich.AccessCategory(Specialty) = FMath::Clamp(m_Rich.AccessCategory(Specialty) - Percent, -1.0, 1.0); break;
	default: return;
	}
}
*/
void City::UpdateDemand(const TEnumAsByte<EWealthLevels>& WealthLevels, const TEnumAsByte<ECitySpecialty>& Specialty, int32 Count)
{
	int32 Index { int(Specialty) };

#ifdef DEBUG_ONLY

	checkf(WealthLevels != EWealthLevels::DefaultWealthEnum,
		TEXT("Error in City::UpdateDemand, WealthLevels == WealthLevels::DefaultWealthEnum."));

	checkf(Index >= 0 && Index < 7, TEXT("Error in City::UpdateDemand, false conversion from Specialty to Index."));

	checkf(DemandModifier != 0, TEXT("Error in City::UpdateDemand, DemandModifier == 0. Check this value is set in MainGameState BeginPlay."));

#endif

	int32 Total{ m_CityLevel * DemandModifier };
	float Percent{ float(Count) / Total };

	switch (WealthLevels)
	{
	case EWealthLevels::Poor: m_DemandPoor[Index] = FMath::Clamp(m_DemandPoor[Index] - Percent, -1.0f, 1.0f); break;
	case EWealthLevels::Middle: m_DemandMiddle[Index] = FMath::Clamp(m_DemandMiddle[Index] - Percent, -1.0f, 1.0f); break;
	case EWealthLevels::Rich: m_DemandRich[Index] = FMath::Clamp(m_DemandRich[Index] - Percent, -1.0f, 1.0f); break;
	default: return;
	}
}


void City::SortBuildingArrayBy(TArray<TWeakObjectPtr<AResidentialBuilding>>& Arr, const TEnumAsByte<ESortingBy>& SortingType)
{

#ifdef DEBUG_ONLY

	checkf(false, TEXT("Error in City::SortBuildingArrayBy, function shouldn't be called."));

#endif 

	switch (SortingType)
	{
	case ESortingBy::Satisfaction:

		// Sort m_ResBuilArr by descending Satisfaction
		Algo::Sort(m_ResBuilArr, [](const TWeakObjectPtr<AResidentialBuilding>& ObjPtr_lhs,
			const TWeakObjectPtr<AResidentialBuilding>& ObjPtr_rhs) -> bool {
				if (!ObjPtr_lhs.IsValid() || !ObjPtr_rhs.IsValid())
				return false;
				else
					return ObjPtr_lhs.Get()->GetInfosResidential().m_SatisfactionPercent >= 
					ObjPtr_rhs.Get()->GetInfosResidential().m_SatisfactionPercent;
			});

		break;
	}
}

float& Demand::AccessCategory(const TEnumAsByte<ECitySpecialty>& Category)
{

#ifdef DEBUG_ONLY

	checkf(Category != ECitySpecialty::DefaultCitySpecialtyEnum,
		TEXT("Error in Demand::ModifyCat, Category != ECitySpecialty::DefaultCitySpecialtyEnum."));

#endif

	switch (Category)
	{
	case Industry: return m_Industry; break;
	case Finance: return m_Finance; break;
	case Science: return m_Science; break;
	case Tourism: return m_Tourism; break;
	case Crime: return m_Crime; break;
	case Military: return m_Military; break;
	case Spiritual: return m_Spirtual; break;
	default: return  m_Industry;  break;
	}
}

void Demand::SetAllValues(float Value)
{
	m_Industry = Value;
	m_Finance = Value;
	m_Science = Value;
	m_Tourism = Value;
	m_Crime = Value;
	m_Military = Value;
	m_Spirtual = Value;
}
