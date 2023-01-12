// Fill out your copyright notice in the Description page of Project Settings.


#include "LittleBigTown/Core/Structs.h"
#include "LittleBigTown/Actors/Building.h"

FText FBuildingInfosBase::GetWealthLevelAsText() const
{
    switch (m_WealthLevel)
    {
    case Poor: return FText::FromString(TEXT("Pauvre ($)")); break;
    case Middle : return FText::FromString(TEXT("Moyen ($$)")); break;
    case Rich : return FText::FromString(TEXT("Riche ($$$)")); break;
    default : return FText::FromString(TEXT("Erreur WealthLevel")); break;
    }
}
