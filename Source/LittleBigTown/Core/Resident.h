// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LittleBigTown/Core/Enums.h"

class Resident
{
public:

	Resident(const TEnumAsByte<enum ECitySpecialty>& Type) : m_Type(Type) {}

	// ~Resident();

protected : 

	TEnumAsByte<enum ECitySpecialty> m_Type;

	double m_Satisfaction { 0.50 };
};
