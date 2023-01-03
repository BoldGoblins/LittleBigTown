// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Misc/DateTime.h"

#include "MainGameState.generated.h"

#define FIRST_YEAR 2000
#define CLOCK_SPEED 10


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTimeUpdatedSignature, int32, Hours, int32, Minutes);


UCLASS()
class LITTLEBIGTOWN_API AMainGameState : public AGameStateBase
{
	GENERATED_BODY()

public :

		void Tick(float DeltaTime) override;

		void BeginPlay() override;

		const FDateTime& GetGameClock() { return GameClock; }

		// return the Month name after checking the GameClock
		// Names are set into the Editor in the BP derivated class of the MainGameState
		FText GetGameClockMonth();

		UPROPERTY(BlueprintAssignable)
			FOnTimeUpdatedSignature OnTimeUpdatedDelegate;



protected :
	
	// Called every tick for Updating GameClock from DeltaTime
	void UpdateClock(float DeltaTime);

	UPROPERTY(BlueprintReadOnly)
		FDateTime GameClock { FIRST_YEAR, 1, 1 };

	// Months names set in the Editor and used by GetGameClockMonth to display in UI_Main
	UPROPERTY(EditDefaultsOnly, Category = "Clock")
		TArray <FText> MonthsNames;

	// Coefficient applied to DeltaTime in UpdateClock to control Clock speed
	UPROPERTY(EditDefaultsOnly, Category = "Clock")
		int32 ClockSpeed { CLOCK_SPEED };
	
};
