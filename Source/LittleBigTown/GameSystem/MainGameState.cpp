// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameState.h"

void AMainGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateClock(DeltaTime);
}
// Called before NewMonthDelegate Broadcast
void AMainGameState::PreMonthlySetup()
{
	TotalPopulation = 0;
	TotalIncomes = 0;

	for (const auto Res : ResBuilArr)
	{
		// Before new m_Incomes is calculated for new month
		if (Res.IsValid())
			TotalMoney += (Res->GetInfosResidential().m_Incomes - Res->GetInfosBase().m_Outgoings);
	}
}

void AMainGameState::PostMonthlySetup()
{
	for (const auto Res : ResBuilArr)
	{
		if (Res.IsValid())
		{
			TotalPopulation += Res->GetInfosBase().m_OccupationCurrentCount;
			TotalIncomes += (Res->GetInfosResidential().m_Incomes - Res->GetInfosBase().m_Outgoings);
		}
	}
}

void AMainGameState::BeginPlay()
{
	Super::BeginPlay();

	TArray <AActor*> Arr{};
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AResidentialBuilding::StaticClass(), Arr);
	// int32 PopCount { 0 };

	for (const auto Elem : Arr)
	{
		const auto Building{ Cast <AResidentialBuilding>(Elem) };

		if (Building)
		{
			ResBuilArr.Add(Building);
			Building->OnResBuildingInfosChangedDelegate.AddDynamic(this, &ThisClass::UpdateInformations);
			TotalPopulation += Building->GetInfosBase().m_OccupationCurrentCount;
			TotalIncomes += (Building->GetInfosResidential().m_Incomes - Building->GetInfosBase().m_Outgoings);
		}
	}

	// TotalPopulation = PopCount;
}

void AMainGameState::UpdateInformations(const FBuildingInfosBase& BaseInfos, const FResidentialBuildingInfos& ResInfos)
{
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
			ResBuilArr.Add(Res);
			Res->OnResBuildingInfosChangedDelegate.AddDynamic(this, &ThisClass::UpdateInformations);
			TotalPopulation += Res->GetInfosBase().m_OccupationCurrentCount;
			TotalIncomes += (Res->GetInfosResidential().m_Incomes - Res->GetInfosBase().m_Outgoings);
		}
	}
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
		PostMonthlySetup();
	}

	if (TempHour != GameClock.GetHour())
		OnNewHourDelegate.Broadcast(GameClock.GetHour());

	if (TempMin != GameClock.GetMinute())
		OnNewMinuteDelegate.Broadcast(GameClock.GetMinute());

}
