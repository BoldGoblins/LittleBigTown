// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Misc/DateTime.h"
// GetAllActors
#include "Kismet/GameplayStatics.h"
#include "LittleBigTown/Actors/Building.h"
#include "LittleBigTown/Actors/ResidentialBuilding.h"
#include "LittleBigTown/GameSystem/City.h"
#include "LittleBigTown/Core/SocialClass.h"

#include "MainGameState.generated.h"

#define FIRST_YEAR "2000"
#define FIRST_CLOCK_YEAR 2000
#define FIRST_MONTH "Janvier"
#define FIRST_TIME "00:00"
#define CLOCK_SPEED 10
#define DEMAND_MODIFIER 1000

// Demand arrays indices :
#define INDICE_INDUSTRIAL 0
#define INDICE_FINANCE 1
#define INDICE_SCIENCE 2
#define INDICE_TOURISM 3
#define INDICE_CRIME 4
#define INDICE_MILITARY 5
#define INDICE_SPIRITUAL 6


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

		const TArray <TWeakObjectPtr<AResidentialBuilding>>& GetBuildingResArray() { return City.m_ResBuilArr; }

		const TMap <ECitySpecialty, float>& GetSpecialtiesFrequencies(const TEnumAsByte<EWealthLevels>& WealthLevels) const;

		// const TArray <int>& GetSpecialtiesFreqCumul(const TEnumAsByte<EWealthLevels>& WealthLevels) const;

		void AddOrSubResidents(const TEnumAsByte<EWealthLevels>& WealthLevels, 
			const TEnumAsByte<ECitySpecialty>& Specialty, int32 Count, int32 IncomePerHabitant);

		UFUNCTION(BlueprintCallable)
			void UnlockSocialClass(const TEnumAsByte <EWealthLevels>& Wealth, const TEnumAsByte <ECitySpecialty>& Specialty, float DefaultDemand = 0.5f);

		const FSocialClasses& GetSocialClasses (const TEnumAsByte<EWealthLevels>& WealthLevel) const;

		const TArray <FText>& GetSpecialtyNames(const TEnumAsByte<enum EWealthLevels>& Wealth) const;

		UFUNCTION(BlueprintCallable)
			void AddNewBuilding(ABuilding* Building);

		int32 GetPopulationCount() const { return City.m_TotalPopulation; }
		int32 GetPopulationVariation() const { return City.m_PopulationVariation; }
		int32 GetTotalMoney() const { return TotalMoney; }
		int32 GetIncomes() const { return TotalIncomes; }

		float GetResidentialDemand(const TEnumAsByte<EWealthLevels>& WealthLevels) const;

		UFUNCTION(BlueprintCallable)
			void DebugSetResidentialDemand(float Poor, float Middle, float Rich);

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
	// Optimisation : mettre le TArray <int> des fréquences cumulées de la demande en variable membre de la classe et le mettre à jour ici
	// Transmettre ensuite par const ref aux Residential Buildings
	void PreHourSetup();

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


	class AMainGameMode* MainGameMode { nullptr };



	// --------------------------------------		Game properties		--------------------------------------


	UPROPERTY(EditDefaultsOnly, Category = "City properties")
		int32 DemandModifier { DEMAND_MODIFIER };

	UPROPERTY(BlueprintReadOnly)
		int32 TotalMoney { 0 };

	UPROPERTY(BlueprintReadOnly)
		int32 TotalIncomes { 0 };

	UPROPERTY(BlueprintReadOnly)
		int32 TotalOutgoings { 0 };

	UPROPERTY(EditDefaultsOnly, Category = "SocialClasses")
		FSocialClasses m_SC_Poor;

	UPROPERTY(EditDefaultsOnly, Category = "SocialClasses")
		FSocialClasses m_SC_Middle;

	UPROPERTY(EditDefaultsOnly, Category = "SocialClasses")
		FSocialClasses m_SC_Rich;

	City City{};	
};