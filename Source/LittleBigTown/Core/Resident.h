// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LittleBigTown/Core/Enums.h"
#include "LittleBigTown/Core/SocialClass.h"
#include "LittleBigTown/Actors/Pedestrian.h"

struct FResident
{

	FResident(const TEnumAsByte <ECitySpecialty>& Type, const FSocialClasses & SocialClass) : m_Type { Type },
		m_SubClassName { SocialClass.GetRandomSubClass(Type).ToString() }, m_Needs{SocialClass.GetMap(Type)[m_SubClassName]} 
	{

	}

	// const TEnumAsByte<enum ECitySpecialty>& GetType() { return m_Type; }

	~FResident();

	ECitySpecialty m_Type;

	FName m_SubClassName;

	FNeeds m_Needs;

	float m_Satisfaction { 0.50 };

	// Pedestrian associated with this Resident
	// Delete it in the Resident's Destructor to ensure a "Ghost" representation of this Resident doesn't still be walking into the town...
	TWeakObjectPtr <APedestrian> _Pedestrian { nullptr };
};
