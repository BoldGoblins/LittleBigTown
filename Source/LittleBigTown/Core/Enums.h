// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Enums.generated.h"

UENUM(BlueprintType)
enum ESlotType
{
	Residential, Commercial, Offices, Industrial, Special, DefaultTypeEnum
};

UENUM(BlueprintType)
enum ESlotSize
{
	Small, Medium, Large, NoSize, DefaultSizeEnum
};

UENUM(BlueprintType)
enum EWealthLevels
{
	Poor, Middle, Rich, DefaultWealthEnum
};

UENUM(BlueprintType)
enum ECitySpecialty
{
	Industry, Finance, Science, Tourism, Crime, Military, Spiritual, DefaultCitySpecialtyEnum
};

enum ESortingBy
{
	Occupation, Satisfaction
};