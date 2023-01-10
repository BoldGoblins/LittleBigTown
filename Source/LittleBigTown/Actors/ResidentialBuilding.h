// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Building.h"
// struct residential infos definition
#include "LittleBigTown/Core/Structs.h"

#include "ResidentialBuilding.generated.h"

#define MAX_LEVEL_RESIDENTIAL 5
#define MIN_COUNT_RESIDENTIAL FMath::RoundToInt ((InfosBase.m_OccupationMaxCount * 10) / 100)
#define OCCUPATION_VARIATION_FACTOR 1.5f
#define BASE_INCOMES 50
#define BASE_SATISFACTION 1.0f

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnResBuildingInfosChangedSignature, const FBuildingInfosBase&, BaseInfos, const FResidentialBuildingInfos&, ResInfos);

UCLASS()
class LITTLEBIGTOWN_API AResidentialBuilding : public ABuilding
{
	GENERATED_BODY()

public :

	UFUNCTION()
		void UpdateBuildingMonthlyInformations(int32 Month);

	UFUNCTION(BlueprintCallable)
		void AddSatisfaction(float Satisfaction);

	UFUNCTION(BlueprintCallable)
		const FResidentialBuildingInfos & GetInfosResidential() const { return ResidentialInformations; }

	FOnResBuildingInfosChangedSignature OnResBuildingInfosChangedDelegate;


	void BeginPlay() override;

protected : 

	UPROPERTY(EditDefaultsOnly)
		int32 ResidentsMaxCount { 0 };

	UPROPERTY(EditDefaultsOnly)
		int32 Current_Outgoings;

	UPROPERTY(EditDefaultsOnly)
		int32 IncomePerHabitant { BASE_INCOMES };

	UPROPERTY(EditDefaultsOnly)
		float VariationOccupationFactor { OCCUPATION_VARIATION_FACTOR };

	FResidentialBuildingInfos ResidentialInformations;
	
};
