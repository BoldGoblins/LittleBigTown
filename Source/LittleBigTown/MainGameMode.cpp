// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameMode.h"

const TMap<FName, FBuildingContainers> & AMainGameMode::GetBuildingsMap(TEnumAsByte <ESlotType> SlotType, TEnumAsByte <ESlotSize> SlotSize) const
{
	// To return something if SlotType/Size are incorrects.
	// Cannot be return because check for Type/Size is proceed before calling this function (in Thematic Widget)
	TMap<FName, FBuildingContainers> NullReturn;


	switch (SlotType)
	{

#ifdef DEBUG_ONLY
		checkf(SlotType != ESlotType::DefaultTypeEnum,
			TEXT("Erreur AMainGameMode::GetBuildingsMap, SlotType == DefaultTypeEnum."));
#endif

	case ESlotType::Residential :

#ifdef DEBUG_ONLY
		checkf(SlotSize != ESlotSize::NoSize && SlotSize != ESlotSize::DefaultSizeEnum,
			TEXT("Erreur AMainGameMode::GetBuildingsMap, SlotSize incompatible with Type Residential."));
#endif

		switch (SlotSize)
		{
		case ESlotSize::Small : return S_ResidentialMap;
		case ESlotSize::Medium : return M_ResidentialMap;
		case ESlotSize::Large : return L_ResidentialMap;
		default : break;
		}

	case ESlotType::Commercial :

#ifdef DEBUG_ONLY
		checkf(SlotSize != ESlotSize::NoSize && SlotSize != ESlotSize::DefaultSizeEnum,
			TEXT("Erreur AMainGameMode::GetBuildingsMap, SlotSize incompatible with Type Commercial."));
#endif

		switch (SlotSize)
		{
		case ESlotSize::Small: return S_CommercialMap;
		case ESlotSize::Medium: return M_CommercialMap;
		case ESlotSize::Large: return L_CommercialMap;
		default: break;
		}

	case ESlotType::Offices :

#ifdef DEBUG_ONLY
		checkf(SlotSize != ESlotSize::NoSize && SlotSize != ESlotSize::DefaultSizeEnum,
			TEXT("Erreur AMainGameMode::GetBuildingsMap, SlotSize incompatible with Type Offices."));
#endif

		switch (SlotSize)
		{
		case ESlotSize::Small: return S_OfficesMap;
		case ESlotSize::Medium: return M_OfficesMap;
		case ESlotSize::Large: return L_OfficesMap;
		default: break;
		}

	case ESlotType::Industrial :

#ifdef DEBUG_ONLY
		checkf(SlotSize == ESlotSize::NoSize,
			TEXT("Erreur AMainGameMode::GetBuildingsMap, SlotSize incompatible with Type Industrial."));
#endif

		return IndustrialMap;

	default: break;
	}

	return NullReturn;
}

