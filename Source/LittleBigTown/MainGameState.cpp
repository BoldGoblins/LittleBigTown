// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameState.h"

void AMainGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateClock(DeltaTime);
	// GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, GameClock.ToString());
}

void AMainGameState::BeginPlay()
{
	Super::BeginPlay();

}

float AMainGameState::GetMonthPercentage()
{
	int32 TotalDays{ GameClock.DaysInMonth(GameClock.GetYear(), GameClock.GetMonth()) };

	int32 SumMinutes{ GameClock.GetMinute() + (GameClock.GetHour() * 60) + ((GameClock.GetDay() - 1) * 24 * 60) };

	int32 TotalMinutes{ TotalDays * 24 * 60 };

	return float(SumMinutes) / TotalMinutes;
}

void AMainGameState::UpdateClock(float DeltaTime)
{
	GameClock += FTimespan::FromSeconds(DeltaTime * 60 * ClockSpeed);
}
