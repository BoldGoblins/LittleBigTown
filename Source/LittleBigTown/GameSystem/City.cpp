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
	// m_DemandPoor = TMap <TEnumAsByte <ECitySpecialty>, float> { {ECitySpecialty::Industry, 0.5f}, {ECitySpecialty::Crime, 0.5f} };
	// m_DemandMiddle = TMap <TEnumAsByte <ECitySpecialty>, float>{ {ECitySpecialty::Industry, 0.5f}, {ECitySpecialty::Crime, 0.5f} };
	// m_DemandRich = TMap <TEnumAsByte <ECitySpecialty>, float>{ {ECitySpecialty::Industry, 0.5f}, {ECitySpecialty::Crime, 0.5f}, {ECitySpecialty::Finance, 0.5f} };
	m_DemandPoor = TMap <ECitySpecialty, float>{ {ECitySpecialty::Industry, 0.5f}, {ECitySpecialty::Crime, 0.5f} };
	m_DemandMiddle = TMap <ECitySpecialty, float>{ {ECitySpecialty::Industry, 0.5f}, {ECitySpecialty::Crime, 0.5f} };
	m_DemandRich = TMap <ECitySpecialty, float>{ {ECitySpecialty::Industry, 0.5f}, {ECitySpecialty::Crime, 0.5f}, {ECitySpecialty::Finance, 0.5f} };
	// GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("%d"), m_DemandRich.Num()));
}

void City::UpdateDemand(const TEnumAsByte<EWealthLevels>& WealthLevels, const TEnumAsByte<ECitySpecialty>& Specialty, int32 Count)
{

#ifdef DEBUG_ONLY

	checkf(WealthLevels != EWealthLevels::DefaultWealthEnum,
		TEXT("Error in City::UpdateDemand, WealthLevels == WealthLevels::DefaultWealthEnum."));

	checkf(DemandModifier != 0, TEXT("Error in City::UpdateDemand, DemandModifier == 0. Check this value is set in MainGameState BeginPlay."));

#endif

	int32 Total{ m_CityLevel * DemandModifier };
	float Percent{ float(Count) / Total };

	switch (WealthLevels)
	{
	case EWealthLevels::Poor: m_DemandPoor[Specialty] = FMath::Clamp(m_DemandPoor[Specialty] - Percent, -1.0f, 1.0f); break;
	case EWealthLevels::Middle: m_DemandMiddle[Specialty] = FMath::Clamp(m_DemandMiddle[Specialty] - Percent, -1.0f, 1.0f); break;
	case EWealthLevels::Rich: m_DemandRich[Specialty] = FMath::Clamp(m_DemandRich[Specialty] - Percent, -1.0f, 1.0f); break;
	default: return;
	}
}

const TMap<ECitySpecialty, float>& City::GetDemand(const TEnumAsByte<EWealthLevels>& WealthLevels) const
{

#ifdef DEBUG_ONLY

	checkf(WealthLevels != EWealthLevels::DefaultWealthEnum,
		TEXT("Error in City::GetDemand, WealthLevels == WealthLevels::DefaultWealthEnum."))

#endif

	switch (WealthLevels)
	{
	case EWealthLevels::Poor: return m_DemandPoor;  break;
	case EWealthLevels::Middle: return m_DemandMiddle; break;
	case EWealthLevels::Rich: return m_DemandRich; break;
	default: return m_DemandPoor;  break;
	}
}

TMap<ECitySpecialty, float>& City::AccessDemand(const TEnumAsByte<EWealthLevels>& WealthLevels)
{
#ifdef DEBUG_ONLY

	checkf(WealthLevels != EWealthLevels::DefaultWealthEnum,
		TEXT("Error in City::AccessDemand, WealthLevels == WealthLevels::DefaultWealthEnum."))

#endif

		switch (WealthLevels)
		{
		case EWealthLevels::Poor: return m_DemandPoor;  break;
		case EWealthLevels::Middle: return m_DemandMiddle; break;
		case EWealthLevels::Rich: return m_DemandRich; break;
		default: return m_DemandPoor;  break;
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