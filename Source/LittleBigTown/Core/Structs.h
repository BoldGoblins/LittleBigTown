// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"


#include "Structs.generated.h"

#define LOCTEXT_NAMESPACE "TempoTry"
#define BASE_INCOMES 50
#define BASE_SATISFACTION 1.0f
#define BASE_OCCUPATION_MAX 5

// --------------------------------------		Buildings		--------------------------------------


// As we cannot use Containers of Containers (TMap of TArrays for each building sub-type for example)
// We need to make a Map of FBuildingContainers that can hold more than just a building as a value
// ComboBoxOptionType is used to populate ScrollBox only with Types that are concerned by the Selected Option of the ComboBox

USTRUCT(BlueprintType)
struct FBuildingContainers
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FText ComboBoxOptionType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf <class ABuilding> Building;
};

USTRUCT(BlueprintType)
struct FBuildingInfosBase
{
	GENERATED_USTRUCT_BODY()

	FText GetWealthLevelAsText() const;

	UPROPERTY(EditDefaultsOnly)
		FText m_Name {};
	// Ne pas définir l'owner directement dans les infos du building à l'avenir
	// Le chopper depuis le PC par exemple
	UPROPERTY(EditDefaultsOnly)
		FText m_Owner{ LOCTEXT("Sarkozy", "Sarkozy") };

	UPROPERTY(EditDefaultsOnly)
		FText m_Description{};

	UPROPERTY(EditDefaultsOnly)
		int32 m_CurrentLevel{ 1 };

	UPROPERTY(EditDefaultsOnly)
		int32 m_MaxLevel{};
	
	UPROPERTY(EditDefaultsOnly)
		int32 m_OccupationMaxCount{ BASE_OCCUPATION_MAX };

	int32 m_OccupationCurrentCount{};

	UPROPERTY(EditDefaultsOnly)
		int32 m_Outgoings{};

	UPROPERTY(EditDefaultsOnly)
		TEnumAsByte <EWealthLevels>  m_WealthLevel{ DefaultWealthEnum };

	FText m_DistrictName{};
};

USTRUCT(BlueprintType)
struct FResidentialBuildingInfos 
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly)
		float m_SatisfactionPercent{ BASE_SATISFACTION };

	UPROPERTY(EditDefaultsOnly)
		int32 m_IncomesPerHab { BASE_INCOMES };

	int32 m_TotalIncomes{};
};

USTRUCT(BlueprintType)
struct FNeeds
{
	GENERATED_USTRUCT_BODY()

	friend FNeeds operator+(const FNeeds& Lhs, const FNeeds& Rhs);

	friend FNeeds operator/(const FNeeds& Lhs, int32 X);

	FNeeds& operator= (const FNeeds& Rhs);

	FNeeds& operator += (const FNeeds& Rhs);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float m_FoodAndDrinks { 0.0 };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float m_Entertainment { 0.0 };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float m_Social { 0.0 };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float m_Health { 0.0 };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float m_Retail { 0.0 };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float m_Luxury { 0.0 };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float m_Tourism { 0.0 };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float m_Culture { 0.0 };
};


#undef LOCTEXT_NAMESPACE