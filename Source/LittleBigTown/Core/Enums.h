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