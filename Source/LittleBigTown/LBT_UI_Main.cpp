// Fill out your copyright notice in the Description page of Project Settings.


#include "LBT_UI_Main.h"

void ULBT_UI_Main::GetClockUpdate(int32 Hour, int32 Minutes)
{
	// NB : If perfs are to low, consider Append FDateTime components one by one in a new FString

	FString S { GameState->GetGameClock().ToString() };
	int32 i;

	// Remove date
	S.FindChar('-', i);

	FString Ret{ S.Mid(i + 1) };

	// Replace '.' in time display
	Ret.FindChar('.', i);
	Ret[i] = ':';

	// Remove seconds in time display
	Ret.FindChar('.', i);
	Ret = Ret.Mid(0, i);

	// Append date
	Ret.Append(" - ");
	Ret.Append(GameState->GetGameClockMonth().ToString());
	Ret.Append(" ");
	Ret.Append(FString::FromInt(GameState->GetGameClock().GetYear()));

	TextBlock->SetText(FText::FromString(Ret));

	// 1439 is maximum amont of minutes per days
	ProgressBar->SetPercent(float(Hour * 60 + Minutes) / 1439);
}

void ULBT_UI_Main::NativeConstruct()
{
	Super::NativeConstruct();

	GameState = Cast <AMainGameState> (UGameplayStatics::GetGameState(GetWorld()));
	GameState->OnTimeUpdatedDelegate.AddDynamic(this, &ThisClass::GetClockUpdate);
}

void ULBT_UI_Main::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

}
