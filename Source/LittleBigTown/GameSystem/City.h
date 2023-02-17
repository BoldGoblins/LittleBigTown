// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
// WealthsLevel, SortingBy
#include "LittleBigTown/Core/Enums.h"
#include "LittleBigTown/LittleBigTown.h"

struct City
{
	City();

	int32 m_TotalPopulation { 0 };
	int32 m_CityLevel { 1 };
	int32 m_PopulationVariation {0};
	int32 DemandModifier{0};

	void UpdateDemand(const TEnumAsByte<EWealthLevels>& WealthLevels, const TEnumAsByte<ECitySpecialty>& Specialty, int32 Count);

	const TMap <ECitySpecialty, float> & GetDemand (const TEnumAsByte<EWealthLevels>& WealthLevels) const;

	TMap <ECitySpecialty, float>& AccessDemand(const TEnumAsByte<EWealthLevels>& WealthLevels);

	// TMap <TEnumAsByte <ECitySpecialty>, float> m_DemandPoor{};
	// TMap <TEnumAsByte <ECitySpecialty>, float> m_DemandMiddle{};
	// TMap <TEnumAsByte <ECitySpecialty>, float> m_DemandRich{};

	TMap <ECitySpecialty, float> m_DemandPoor{};
	TMap <ECitySpecialty, float> m_DemandMiddle{};
	TMap <ECitySpecialty, float> m_DemandRich{};

	TArray <TWeakObjectPtr<class AResidentialBuilding>> m_ResBuilArr;

private :

	void SortBuildingArrayBy(TArray <TWeakObjectPtr<AResidentialBuilding>>& Arr, const TEnumAsByte<ESortingBy>& SortingType);

};



