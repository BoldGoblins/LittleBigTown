// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Structs.generated.h"


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

	FText m_Name {};
	int32 m_CurrentLevel{};
	int32 m_MaxLevel{};
	int32 m_OccupationMaxCount{};
	int32 m_OccupationCurrentCount{};
	int32 m_Outgoings{};
};

USTRUCT(BlueprintType)
struct FResidentialBuildingInfos 
{
	GENERATED_USTRUCT_BODY()

	float m_SatisfactionPercent{};
	int32 m_Incomes{};
};

