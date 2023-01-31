// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
// WealthsLevel, SortingBy
#include "LittleBigTown/Core/Enums.h"
#include "LittleBigTown/LittleBigTown.h"

struct Demand
{
	float m_Industry{ 0.5f };
	float m_Finance{ 0.5f };
	float m_Science{ 0.5f };
	float m_Tourism{ 0.5f };
	float m_Crime{ 0.5f };
	float m_Military{ 0.5f };
	float m_Spirtual{ 0.5f };

	float& AccessCategory(const TEnumAsByte<ECitySpecialty>& Category);
	float GetAverage() const { return (m_Industry + m_Finance + m_Science + m_Tourism + m_Crime + m_Military + m_Spirtual) / 7; }

	void SetAllValues(float Value);
};

struct City
{
	City();

	int32 m_TotalPopulation { 0 };
	int32 m_CityLevel { 1 };
	int32 m_PopulationVariation {0};
	int32 DemandModifier{0};

	void UpdateDemand(const TEnumAsByte<EWealthLevels>& WealthLevels, const TEnumAsByte<ECitySpecialty>& Specialty, int32 Count);
	/*
	// Housing Appliants percentage :
	Demand m_Poor {};
	Demand m_Middle {};
	Demand m_Rich {};
	*/

	TArray <float> m_DemandPoor{};
	TArray <float> m_DemandMiddle{};
	TArray <float> m_DemandRich{};

	TArray <TWeakObjectPtr<class AResidentialBuilding>> m_ResBuilArr;

private :

	void SortBuildingArrayBy(TArray <TWeakObjectPtr<AResidentialBuilding>>& Arr, const TEnumAsByte<ESortingBy>& SortingType);

};



