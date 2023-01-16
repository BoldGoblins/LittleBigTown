// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameState.h"
#include "LittleBigTown/Core/Debugger.h"

void AMainGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateClock(DeltaTime);
}
// Called before NewMonthDelegate Broadcast
void AMainGameState::PreMonthlySetup()
{
	TotalMoney += TotalIncomes;
	City.m_PoorDemand = 0.5f, City.m_MiddleDemand = 0.5f, City.m_RichDemand = 0.5f;
}

void AMainGameState::PreHourSetup()
{
	City.m_PopulationVariation = 0;
}

void AMainGameState::BeginPlay()
{
	Super::BeginPlay();

	TArray <AActor*> Arr{};
	UGameplayStatics::GetAllActorsOfClass(GEngine->GameViewport->GetWorld(), AResidentialBuilding::StaticClass(), Arr);

	for (const auto Elem : Arr)
	{
		const auto Building{ Cast <AResidentialBuilding>(Elem) };

		if (Building)
		{
			City.m_ResBuilArr.Add(Building);
			City.m_TotalPopulation += Building->GetInfosBase().m_OccupationCurrentCount;
			TotalIncomes += (Building->GetInfosResidential().m_TotalIncomes - Building->GetInfosBase().m_Outgoings);
			TotalOutgoings += Building->GetInfosBase().m_Outgoings;
		}
	}
	
	City.DemandModifier = DemandModifier;
}

void AMainGameState::AddOrSubResidents(const TEnumAsByte<WealthLevels>& WealthLevels, int32 Count, int32 IncomePerHabitant)
{
	City.m_TotalPopulation += Count;
	City.m_PopulationVariation += Count;
	TotalIncomes += (IncomePerHabitant * Count);
	City.UpdateDemand(WealthLevels, Count);
}

void AMainGameState::AddNewBuilding(ABuilding* Building)
{
	if (!Building)
		return;

	if (Building->IsA <AResidentialBuilding>())
	{
		const auto Res { Cast <AResidentialBuilding> (Building) };

		if (Res)
		{
			City.m_ResBuilArr.Add(Res);
			TotalOutgoings += Building->GetInfosBase().m_Outgoings;
			// Par s�curit�, m�me si normalement nouveau Building vide (= pas d'habitants ni de revenus)...
			City.m_TotalPopulation += Res->GetInfosBase().m_OccupationCurrentCount;
			City.m_PopulationVariation += Res->GetInfosBase().m_OccupationCurrentCount;
			TotalIncomes += (Res->GetInfosResidential().m_TotalIncomes - Res->GetInfosBase().m_Outgoings);
		}
	}
}

float AMainGameState::GetResidentialDemand(const TEnumAsByte<WealthLevels>& WealthLevels) const
{

#ifdef DEBUG_ONLY

	checkf(WealthLevels != WealthLevels::DefaultWealthEnum,
		TEXT("Error in AMainGameState::GetResidentialDemand, WealthLevels == WealthLevels::DefaultWealthEnum."))

#endif

	switch (WealthLevels)
	{
	case WealthLevels::Poor: return City.m_PoorDemand;
	case WealthLevels::Middle: return City.m_MiddleDemand;
	case WealthLevels::Rich: return City.m_RichDemand;
	default: return 0.0f;
	}
}

void AMainGameState::DebugSetResidentialDemand(float Poor, float Middle, float Rich)
{
	City.m_PoorDemand = Poor;
	City.m_MiddleDemand = Middle;
	City.m_RichDemand = Rich;
}

FText AMainGameState::GetGameClockMonth()
{
	// -1 because months starts at 1 and our first index is 0
	auto i { GameClock.GetMonth() - 1 };

	return MonthsNames[i];
}

void AMainGameState::UpdateClock(float DeltaTime)
{
	const int32 TempMin { GameClock.GetMinute() };
	const int32 TempHour { GameClock.GetHour() };

	GameClock += FTimespan::FromSeconds(DeltaTime * 60 * ClockSpeed);

	if (GameClock.GetDay() > 1)
	{
		if (GameClock.GetMonth() == 12)
		{
			GameClock = FDateTime(GameClock.GetYear() + 1, 1, 1, 0, 0, 0);
			OnNewYearDelegate.Broadcast(GameClock.GetYear());
		}
		else
			GameClock = FDateTime(GameClock.GetYear(), GameClock.GetMonth() + 1, 1, 0, 0, 0);

		PreMonthlySetup();
		OnNewMonthDelegate.Broadcast(GameClock.GetMonth());
	}

	if (TempHour != GameClock.GetHour())
	{
		PreHourSetup();
		OnNewHourDelegate.Broadcast(GameClock.GetHour());
	}

	if (TempMin != GameClock.GetMinute())
		OnNewMinuteDelegate.Broadcast(GameClock.GetMinute());
}
