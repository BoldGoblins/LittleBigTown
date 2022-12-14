// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Misc/DateTime.h"

#include "MainGameState.generated.h"

#define FIRST_YEAR 2000
#define CLOCK_SPEED 100


UCLASS()
class LITTLEBIGTOWN_API AMainGameState : public AGameStateBase
{
	GENERATED_BODY()

public :

		void Tick(float DeltaTime) override;

		void BeginPlay() override;

		const FDateTime& GetGameClock() { return GameClock; }

		float GetMonthPercentage();



protected :

	void UpdateClock(float DeltaTime);

	UPROPERTY(BlueprintReadOnly)
		FDateTime GameClock { FIRST_YEAR, 1, 1 };

	UPROPERTY(EditDefaultsOnly, Category = "Clock")
		int32 ClockSpeed { CLOCK_SPEED };
	
};
