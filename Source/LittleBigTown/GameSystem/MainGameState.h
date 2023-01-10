// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Misc/DateTime.h"
// GetAllActors
#include "Kismet/GameplayStatics.h"
#include "LittleBigTown/Actors/Building.h"
#include "LittleBigTown/Actors/ResidentialBuilding.h"

#include "MainGameState.generated.h"

#define FIRST_YEAR "2000"
#define FIRST_CLOCK_YEAR 2000
#define FIRST_MONTH "Janvier"
#define FIRST_TIME "00:00"
#define CLOCK_SPEED 10


// DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTimeUpdatedSignature, int32, Hours, int32, Minutes);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewMonthSignature, int32, Month);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewHourSignature, int32, Hour);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewYearSignature, int32, Year);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewMinuteSignature, int32, Minute);



UCLASS()
class LITTLEBIGTOWN_API AMainGameState : public AGameStateBase
{
	GENERATED_BODY()

public :

		void Tick(float DeltaTime) override;

		void BeginPlay() override;

		const FDateTime& GetGameClock() { return GameClock; }

		const TArray <TWeakObjectPtr<AResidentialBuilding>>& GetBuildingResArray() { return ResBuilArr; }

		UFUNCTION()
			void UpdateInformations(const FBuildingInfosBase& BaseInfos, const FResidentialBuildingInfos& ResInfos);

		UFUNCTION(BlueprintCallable)
			void AddNewBuilding(ABuilding* Building);

		int32 GetPopulationCount() const { return TotalPopulation; }
		int32 GetTotalMoney() const { return TotalMoney; }
		int32 GetIncomes() const { return TotalIncomes; }

		// return the Month name after checking the GameClock
		// Names are set into the Editor in the BP derivated class of the MainGameState
		FText GetGameClockMonth();

		UPROPERTY(BlueprintAssignable)
			FOnNewMinuteSignature OnNewMinuteDelegate;

		UPROPERTY(BlueprintAssignable)
			FOnNewMonthSignature OnNewMonthDelegate;

		UPROPERTY(BlueprintAssignable)
			FOnNewHourSignature OnNewHourDelegate;

		UPROPERTY(BlueprintAssignable)
			FOnNewYearSignature OnNewYearDelegate;

private :

	void PreMonthlySetup();

	void PostMonthlySetup();

protected :
	
	// Called every tick for Updating GameClock from DeltaTime
	void UpdateClock(float DeltaTime);

	UPROPERTY(BlueprintReadOnly)
		FDateTime GameClock { FIRST_CLOCK_YEAR, 1, 1 };

	// Months names set in the Editor and used by GetGameClockMonth to display in UI_Main
	UPROPERTY(EditDefaultsOnly, Category = "Clock")
		TArray <FText> MonthsNames;

	// Coefficient applied to DeltaTime in UpdateClock to control Clock speed
	UPROPERTY(EditDefaultsOnly, Category = "Clock")
		int32 ClockSpeed { CLOCK_SPEED };

	TArray <TWeakObjectPtr<AResidentialBuilding>> ResBuilArr;


	// --------------------------------------		Game properties		--------------------------------------


	UPROPERTY(BlueprintReadOnly)
		int32 TotalPopulation { 0 };

	UPROPERTY(BlueprintReadOnly)
		int32 TotalMoney { 0 };

	UPROPERTY(BlueprintReadOnly)
		int32 TotalIncomes { 0 };
	
};
