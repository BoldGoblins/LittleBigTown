// Fill out your copyright notice in the Description page of Project Settings.


#include "ResidentialBuilding.h"
#include "LittleBigTown/UserInterface/UI_BuildingInfos.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "LittleBigTown/GameSystem/MainGameState.h"
#include "LittleBigTown/Core/Debugger.h"


void AResidentialBuilding::BeginPlay()
{
	Super::BeginPlay();

	MainGameSate = Cast <AMainGameState>(UGameplayStatics::GetGameState(GetWorld()));

	// Set Infos values (when EditDefaultsOnly values are set in Editor)
	InfosBase.m_MaxLevel = MAX_LEVEL_RESIDENTIAL;
	InfosBase.m_OccupationMaxCount = ResidentsMaxCount;;
	ResidentialInformations.m_SatisfactionPercent = BASE_SATISFACTION;
	ResidentialInformations.m_IncomesPerHab = IncomePerHabitant;
	ResidentialInformations.m_TotalIncomes = IncomePerHabitant * InfosBase.m_OccupationCurrentCount * CurrentLevel;

	if(MainGameSate)
		MainGameSate->OnNewHourDelegate.AddDynamic(this, &ThisClass::UpdateNewHour);

}

void AResidentialBuilding::UpdateNewHour(int32 Hour)
{
	float Demand { MainGameSate->GetResidentialDemand(InfosBase.m_WealthLevel) };
	int32 OccupationVar { FMath::RoundToInt((Demand * TEN_PERCENT_OCCUPATION) + ((ResidentialInformations.m_SatisfactionPercent - 0.5) * TEN_PERCENT_OCCUPATION)) };

	// GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("Demand : %f, Satisfaction : %f, Variation : %d"),
		// Demand, ResidentialInformations.m_SatisfactionPercent, OccupationVar));
	
	if (InfosBase.m_OccupationCurrentCount + OccupationVar > InfosBase.m_OccupationMaxCount)
		OccupationVar = InfosBase.m_OccupationMaxCount - InfosBase.m_OccupationCurrentCount;

	InfosBase.m_OccupationCurrentCount += OccupationVar;
	ResidentialInformations.m_TotalIncomes += OccupationVar * ResidentialInformations.m_IncomesPerHab;
	MainGameSate->AddOrSubResidents(InfosBase.m_WealthLevel, OccupationVar, ResidentialInformations.m_IncomesPerHab);

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
