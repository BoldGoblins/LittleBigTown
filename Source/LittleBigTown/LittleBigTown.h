// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ComboBoxString.h"
#include "Building.h"
#include "LittleBigTown.generated.h"

// DEFINITIONS : 


// --------------------------------------		DEBUG		--------------------------------------


#define DEBUG_ONLY


// --------------------------------------		Camera/pawn/playercontroller movements		--------------------------------------


#define ZOOM_MIN 10
#define ZOOM_MAX 1

#define ZOOM_FACTOR_MIN 0.800000f
#define ZOOM_FACTOR_MAX 1.000000f 

#define DEFAULT_ZOOM_UNITS 500

// Remember that MaxSpeed/Acceleration = time to get to MaxSpeed
// Exemple : 2 / 200 = 0.01 sec to get to Speed = 2

#define MAX_SPEED 25000,0
#define ACCELERATION 250000,0
#define DECELERATION 250000,0

#define DEFAULT_SPRING_ARM_LENGTH 6000.000000f

#define MIN_PITCH_ANGLE  - 80.0f
#define MAX_PITCH_ANGLE  - 10.0f 

#define DEFAULT_PITCH_ROTATION_PAWN - 50.0f

#define DEFAULT_PAWN_LOCATION_Z_AXIS_MIN 300.0f
#define DEFAULT_PAWN_LOCATION_Z_AXIS_MAX 1800.0f

#define MAX_ALTITUDE_IN_LEVEL 10000


// --------------------------------------		BuildingWidget		--------------------------------------


// #define X_BUTTON_TRANSLATION_MAX 50.0f
#define MAX_SCROLLBOX_BUTTONS 20


// --------------------------------------		ConstructibleSlot		--------------------------------------
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

// As we cannot use Containers of Containers (TMap of TArrays for each building sub-type for example)
// We need to make a Map of FBuildingContainers that can hold more than just a building as a value
// ComboBoxOptionType is used to populate ScrollBox only with Types that are concerned by the Selected Option of the ComboBox

USTRUCT(BlueprintType)
struct FBuildingContainers
{
	GENERATED_USTRUCT_BODY()

public :

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FText ComboBoxOptionType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf <ABuilding> Building;
};

namespace MyPersonalLibrary
{
	// Clear and Refresh the ComboBox automatically
	void AddOptionsToComboBoxString(UComboBoxString* ComboBox, const TArray <FString>& Options);
}
