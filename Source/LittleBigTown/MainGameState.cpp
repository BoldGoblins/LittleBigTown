// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameState.h"

void AMainGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateClock(DeltaTime);
}

void AMainGameState::BeginPlay()
{
	Super::BeginPlay();

}

FText AMainGameState::GetGameClockMonth()
{
	// -1 because months starts at 1 and our first index is 0
	auto i { GameClock.GetMonth() - 1 };

	return MonthsNames[i];
}

void AMainGameState::UpdateClock(float DeltaTime)
{
	GameClock += FTimespan::FromSeconds(DeltaTime * 60 * ClockSpeed);

	if (GameClock.GetDay() > 1)
	{
		if (GameClock.GetMonth() == 12)
			GameClock = FDateTime(GameClock.GetYear() + 1, 1, 1, 0, 0, 0);
		else
			GameClock = FDateTime(GameClock.GetYear(), GameClock.GetMonth() + 1, 1, 0, 0, 0);
	}
	
}
