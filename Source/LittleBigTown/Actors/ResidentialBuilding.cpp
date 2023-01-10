// Fill out your copyright notice in the Description page of Project Settings.


#include "ResidentialBuilding.h"
#include "LittleBigTown/UserInterface/UI_BuildingInfos.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "LittleBigTown/GameSystem/MainGameState.h"


void AResidentialBuilding::BeginPlay()
{
	Super::BeginPlay();

	// Set Infos values (when EditDefaultsOnly values are set in Editor)
	InfosBase.m_Name = Name;
	InfosBase.m_CurrentLevel = CurrentLevel;
	InfosBase.m_MaxLevel = MAX_LEVEL_RESIDENTIAL;
	InfosBase.m_OccupationMaxCount = ResidentsMaxCount;
	InfosBase.m_OccupationCurrentCount = MIN_COUNT_RESIDENTIAL;
	InfosBase.m_Outgoings = Current_Outgoings;
	ResidentialInformations.m_SatisfactionPercent = BASE_SATISFACTION;
	ResidentialInformations.m_Incomes = IncomePerHabitant * InfosBase.m_OccupationCurrentCount * CurrentLevel;

	const auto MainGS{ Cast <AMainGameState>(UGameplayStatics::GetGameState(GetWorld())) };

	if (MainGS)
	{
		MainGS->OnNewMonthDelegate.AddDynamic(this, &ThisClass::UpdateBuildingMonthlyInformations);
	}

}

void AResidentialBuilding::UpdateBuildingMonthlyInformations(int32 Month)
{

	// Habitants variation : 
	{
		int32 OccupVar{ FMath::RoundToInt(MIN_COUNT_RESIDENTIAL * VariationOccupationFactor *
	(float(ResidentialInformations.m_SatisfactionPercent - 0.5f))) };

		InfosBase.m_OccupationCurrentCount = FMath::Clamp(InfosBase.m_OccupationCurrentCount + OccupVar,
			MIN_COUNT_RESIDENTIAL, InfosBase.m_OccupationMaxCount);
	}

	// Incomes variation :
	ResidentialInformations.m_Incomes = IncomePerHabitant * InfosBase.m_OccupationCurrentCount * InfosBase.m_CurrentLevel;

	// Declare Building Infos Modification :
	OnResBuildingInfosChangedDelegate.Broadcast(InfosBase, ResidentialInformations);
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
	OnResBuildingInfosChangedDelegate.Broadcast(InfosBase, ResidentialInformations);
}
