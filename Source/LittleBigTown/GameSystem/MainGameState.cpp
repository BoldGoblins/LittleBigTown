// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameState.h"
#include "LittleBigTown/Core/Debugger.h"
// #include "Algo/Accumulate.h"
#include "LittleBigTown/GameSystem/MainGameMode.h"
#include "Kismet/GameplayStatics.h"

void AMainGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateClock(DeltaTime);
}
// Called before NewMonthDelegate Broadcast
void AMainGameState::PreMonthlySetup()
{
	TotalMoney += TotalIncomes;
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

	MainGameMode = Cast <AMainGameMode> (UGameplayStatics::GetGameMode(GetWorld()));
}

const TMap <ECitySpecialty, float> & AMainGameState::GetSpecialtiesFrequencies(const TEnumAsByte<EWealthLevels>& WealthLevels) const
{

#ifdef DEBUG_ONLY

	checkf(WealthLevels != EWealthLevels::DefaultWealthEnum,
		TEXT("Error in AMainGameState::GetSpecialtyFrequencies, WealthLevels == WealthLevels::DefaultWealthEnum."));

#endif

	switch (WealthLevels)
	{
	case Poor: return City.m_DemandPoor; break;
	case Middle: return City.m_DemandMiddle; break;
	case Rich: return City.m_DemandRich; break;
	default: return City.m_DemandPoor; break;
	}
}

void AMainGameState::AddOrSubResidents(const TEnumAsByte<EWealthLevels>& WealthLevels, const TEnumAsByte<ECitySpecialty>& Specialty, int32 Count, int32 IncomePerHabitant)
{
	City.m_TotalPopulation += Count;
	City.m_PopulationVariation += Count;
	TotalIncomes += (IncomePerHabitant * Count);
	City.UpdateDemand(WealthLevels, Specialty, Count);
}

void AMainGameState::UnlockSocialClass(const TEnumAsByte<EWealthLevels>& Wealth, const TEnumAsByte<ECitySpecialty>& Specialty, float DefaultDemand)
{

#ifdef DEBUG_ONLY

	checkf(!City.GetDemand(Wealth).Contains(Specialty), 
		TEXT("Error in AMainGameState::UnlockSocialClass : Try to add a Social Class that is already referenced into Map."))

#endif

	if (!City.GetDemand(Wealth).Contains(Specialty))
		City.AccessDemand(Wealth).Add(MakeTuple(Specialty, DefaultDemand));
}

const FSocialClasses& AMainGameState::GetSocialClasses(const TEnumAsByte<EWealthLevels>& WealthLevel) const
{

#ifdef DEBUG_ONLY

	checkf(WealthLevel != EWealthLevels::DefaultWealthEnum, TEXT("Error in AMainGameState::GetSocialClasses, WealthLevel == Default."));

#endif

	switch (WealthLevel)
	{
	case Poor: return m_SC_Poor; break;
	case Middle: return m_SC_Middle; break;
	case Rich: return m_SC_Rich; break;
	default: return m_SC_Poor; break;
	}
}

const TArray<FText>& AMainGameState::GetSpecialtyNames(const TEnumAsByte<enum EWealthLevels>& Wealth) const
{
	return MainGameMode->GetSpecialtyNames(Wealth);

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
			// Par sécurité, même si normalement nouveau Building vide (= pas d'habitants ni de revenus)...
			City.m_TotalPopulation += Res->GetInfosBase().m_OccupationCurrentCount;
			City.m_PopulationVariation += Res->GetInfosBase().m_OccupationCurrentCount;
			TotalIncomes += (Res->GetInfosResidential().m_TotalIncomes - Res->GetInfosBase().m_Outgoings);
		}
	}
}

float AMainGameState::GetResidentialDemand(const TEnumAsByte<EWealthLevels>& WealthLevels) const
{

#ifdef DEBUG_ONLY

	checkf(WealthLevels != EWealthLevels::DefaultWealthEnum,
		TEXT("Error in AMainGameState::GetResidentialDemand, WealthLevels == WealthLevels::DefaultWealthEnum."))

#endif

	float Sum { 0.0f };

	for (const auto element : City.GetDemand(WealthLevels))
		Sum += element.Value;

	return Sum / City.GetDemand(WealthLevels).Num();
}

void AMainGameState::DebugSetResidentialDemand(float Poor, float Middle, float Rich)
{
	for (auto element : City.m_DemandPoor)
		element.Value = Poor;

	for (auto element : City.m_DemandMiddle)
		element.Value = Middle;

	for (auto element : City.m_DemandRich)
		element.Value = Rich;
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
