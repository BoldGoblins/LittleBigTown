// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LittleBigTown/Core/Enums.h"
#include "LittleBigTown/Core/SocialClass.h"

struct FResident
{

	FResident(const TEnumAsByte<enum ECitySpecialty>& Type, const FSocialClasses & SocialClass) : m_Type { Type },
		m_SubClassName { SocialClass.GetRandomSubClass(Type) }, m_Needs { SocialClass.GetMap(Type)[m_SubClassName] } {}

	const TEnumAsByte<enum ECitySpecialty>& GetType() { return m_Type; }

	// ~Resident();

	TEnumAsByte<enum ECitySpecialty> m_Type;

	FName m_SubClassName;

	FNeeds m_Needs;

	float m_Satisfaction { 0.50 };
};
