// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameMode.h"
// DEBUG_ONLY
#include "LittleBigTown/Core/Debugger.h"

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();

#ifdef DEBUG_ONLY

	checkf(ClassNames.m_PoorNames.Num() == 7 && ClassNames.m_MiddleNames.Num() == 7 && ClassNames.m_RichNames.Num() == 7,
		TEXT("Error in UUI_HappinessInfos::NewDisplay : TArrays.Num() != 7 in SpecialtyNames Arrays."));

#endif
}

const TMap<FName, FBuildingContainers> & AMainGameMode::GetBuildingsMap(TEnumAsByte <ESlotType> SlotType, TEnumAsByte <ESlotSize> SlotSize) const
{
	// To return something if SlotType/Size are incorrects.
	// Cannot be return because check for Type/Size is proceed before calling this function (in Thematic Widget)
	TMap<FName, FBuildingContainers> NullReturn;


	switch (SlotType)
	{

#ifdef DEBUG_ONLY

		checkf(SlotType != ESlotType::DefaultTypeEnum, 
			TEXT("Error in AMainGameMode::GetBuildingsMap, SlotType == DefaultTypeEnum."));

#endif

	case ESlotType::Residential :

#ifdef DEBUG_ONLY

		checkf(SlotSize != ESlotSize::NoSize && SlotSize != ESlotSize::DefaultSizeEnum,
			TEXT("Error in AMainGameMode::GetBuildingsMap, SlotSize incompatible with Type Residential. "));
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

const TArray<FText>& AMainGameMode::GetSpecialtyNames(const TEnumAsByte<enum EWealthLevels>& Wealth) const
{

#ifdef DEBUG_ONLY

	checkf(Wealth != EWealthLevels::DefaultWealthEnum,
		TEXT("Error in AMainGameMode::GetBuildingsMap, SlotType == DefaultTypeEnum."));

#endif

	switch (Wealth)
	{
	case Poor: return ClassNames.m_PoorNames; break;
	case Middle: return ClassNames.m_MiddleNames; break;
	case Rich: return ClassNames.m_RichNames; break;
	default: return ClassNames.m_PoorNames; break;
	}
}

