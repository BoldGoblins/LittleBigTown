// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
// WealthsLevel, SortingBy
#include "LittleBigTown/Core/Enums.h"
#include "LittleBigTown/LittleBigTown.h"


struct City
{
	int32 m_TotalPopulation { 0 };
	int32 m_CityLevel { 1 };
	int32 m_PopulationVariation {0};
	int32 DemandModifier{0};

	void UpdateDemand(const TEnumAsByte<WealthLevels>& WealthLevels, int32 Count);

	// Housing Appliants percentage :
	float m_PoorDemand { 0.5f };
	float m_MiddleDemand { 0.5f };
	float m_RichDemand { 0.5f };

	TArray <TWeakObjectPtr<class AResidentialBuilding>> m_ResBuilArr;

private :

	void SortBuildingArrayBy(TArray <TWeakObjectPtr<AResidentialBuilding>>& Arr, const TEnumAsByte<SortingBy>& SortingType);

};

