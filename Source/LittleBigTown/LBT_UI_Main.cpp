// Fill out your copyright notice in the Description page of Project Settings.


#include "LBT_UI_Main.h"

void ULBT_UI_Main::GetClockUpdate(const FDateTime& GameClock, float MonthPercentage)
{
	if (!ProgressBar)
		return;

	// GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("%f"), float(SumMinutes) / TotalMinutes));
	
	ProgressBar->SetPercent(MonthPercentage);
	TextBlock->SetText(FText::FromString(DateFormat(GameClock)));
}

FString ULBT_UI_Main::DateFormat(const FDateTime& GameClock)
{
	FString S{};

	if(GameClock.GetDay() <= 10)
		S.Append("0").Append(FString::FromInt(GameClock.GetDay()));
	else
		S.Append(FString::FromInt(GameClock.GetDay()));

	S.Append("/");

	if (GameClock.GetMonth() <= 10)
		S.Append("0").Append(FString::FromInt(GameClock.GetMonth()));
	else
		S.Append(FString::FromInt(GameClock.GetMonth()));

	S.Append("/");
	S.Append(FString::FromInt(GameClock.GetYear()));

	return S;
}

void ULBT_UI_Main::NativeConstruct()
{
	Super::NativeConstruct();

	GameState = Cast <AMainGameState> (UGameplayStatics::GetGameState(GetWorld()));
}

void ULBT_UI_Main::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	GetClockUpdate(GameState->GetGameClock(), GameState->GetMonthPercentage());
}
