// Fill out your copyright notice in the Description page of Project Settings.


#include "LittleBigTown/Core/SocialClass.h"
#include "LittleBigTown/Core/Debugger.h"

const TMap<FName, FNeeds>& FSocialClasses::GetMap(const TEnumAsByte<ECitySpecialty>& CitySpecialty) const
{

#ifdef DEBUG_ONLY

	checkf(CitySpecialty != ECitySpecialty::DefaultCitySpecialtyEnum, 
		TEXT("Error in FSocialClasses::GetMap, CitySpecialty == DefaultCitySpecialtyEnum."));

#endif

	switch (CitySpecialty)
	{
	case Industry: return m_Industry; break;
	case Finance: return m_Finance; break;
	case Science: return m_Science; break;
	case Tourism: return m_Tourism; break;
	case Crime: return m_Crime; break;
	case Military: return m_Military; break;
	case Spiritual: return m_Spiritual; break;
	default: return m_Industry; break;
	}
}

FName FSocialClasses::GetRandomSubClass(const TEnumAsByte<ECitySpecialty>& CitySpecialty) const
{
	TArray <FName> Arr{};
	int32 i{ FMath::RandRange(0, Arr.Num() - 1) };

	GetMap(CitySpecialty).GetKeys(Arr);

	return Arr[i];
}
