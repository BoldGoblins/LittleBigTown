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
	/*
	const auto GS{ Cast <AMainGameState> (UGameplayStatics::GetGameState(GEngine->GameViewport->GetWorld())) };

	if(GS)
	*/
}

void City::UpdateDemand(const TEnumAsByte<WealthLevels>& WealthLevels, int32 Count)
{

#ifdef DEBUG_ONLY

	checkf(WealthLevels != WealthLevels::DefaultWealthEnum,
		TEXT("Error in City::UpdateDemand, WealthLevels == WealthLevels::DefaultWealthEnum."))

#endif

	int32 Total { m_CityLevel * 1000 };
	float Percent { float(Count) / Total };

	switch (WealthLevels)
	{
	case WealthLevels::Poor: m_PoorDemand = FMath::Clamp(m_PoorDemand - Percent, -1.0f, 1.0f); break;
	case WealthLevels::Middle: m_MiddleDemand = FMath::Clamp(m_MiddleDemand - Percent, -1.0f, 1.0f); break;
	case WealthLevels::Rich: m_RichDemand = FMath::Clamp(m_RichDemand - Percent, -1.0f, 1.0f); break;
	default: return;
	}
}


void City::SortBuildingArrayBy(TArray<TWeakObjectPtr<AResidentialBuilding>>& Arr, const TEnumAsByte<SortingBy>& SortingType)
{

#ifdef DEBUG_ONLY

	checkf(false, TEXT("Error in City::SortBuildingArrayBy, function shouldn't be called."));

#endif 

	switch (SortingType)
	{
	case SortingBy::Satisfaction:

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




