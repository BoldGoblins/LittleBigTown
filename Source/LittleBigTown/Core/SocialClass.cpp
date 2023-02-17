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
	case ECitySpecialty::Industry: return m_Industry; break;
	case ECitySpecialty::Finance: return m_Finance; break;
	case ECitySpecialty::Science: return m_Science; break;
	case ECitySpecialty::Tourism: return m_Tourism; break;
	case ECitySpecialty::Crime: return m_Crime; break;
	case ECitySpecialty::Military: return m_Military; break;
	case ECitySpecialty::Spiritual: return m_Spiritual; break;
	default: return m_Industry; break;
	}
}

FName FSocialClasses::GetRandomSubClass(const TEnumAsByte<ECitySpecialty>& CitySpecialty) const
{
	TArray <FName> Arr{};
	GetMap(CitySpecialty).GetKeys(Arr);

	int32 i{ FMath::RandRange(0, Arr.Num() - 1) };

	return Arr[i];
}
