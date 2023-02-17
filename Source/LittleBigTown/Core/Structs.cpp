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

FNeeds operator+(const FNeeds& Lhs, const FNeeds& Rhs)
{
    FNeeds Sum{};

    Sum.m_FoodAndDrinks = Lhs.m_FoodAndDrinks + Rhs.m_FoodAndDrinks;
    Sum.m_Retail = Lhs.m_Retail + Rhs.m_Retail;
    Sum.m_Health = Lhs.m_Health + Rhs.m_Health;
    Sum.m_Entertainment = Lhs.m_Entertainment + Rhs.m_Entertainment;
    Sum.m_Social = Lhs.m_Social + Rhs.m_Social;
    Sum.m_Culture = Lhs.m_Culture + Rhs.m_Culture;
    Sum.m_Tourism = Lhs.m_Tourism + Rhs.m_Tourism;
    Sum.m_Luxury = Lhs.m_Luxury + Rhs.m_Luxury;

    return Sum;
}

FNeeds operator/(const FNeeds& Lhs, int32 X)
{
    FNeeds Result{};

    // Division par 0
    if (X == 0)
        return Result;

    Result.m_FoodAndDrinks = Lhs.m_FoodAndDrinks / X;
    Result.m_Retail = Lhs.m_Retail / X;
    Result.m_Health = Lhs.m_Health / X;
    Result.m_Entertainment = Lhs.m_Entertainment / X;
    Result.m_Social = Lhs.m_Social / X;
    Result.m_Culture = Lhs.m_Culture / X;
    Result.m_Tourism = Lhs.m_Tourism / X;
    Result.m_Luxury = Lhs.m_Luxury / X;

    return Result;
}

FNeeds& FNeeds::operator+=(const FNeeds& Rhs)
{
    *this = *this + Rhs;

    return *this;
}

FNeeds& FNeeds::operator=(const FNeeds& Rhs)
{
    m_FoodAndDrinks = Rhs.m_FoodAndDrinks;
    m_Retail = Rhs.m_Retail;
    m_Health = Rhs.m_Health;
    m_Entertainment = Rhs.m_Entertainment;
    m_Social = Rhs.m_Social;
    m_Culture = Rhs.m_Culture;
    m_Tourism = Rhs.m_Tourism;
    m_Luxury = Rhs.m_Luxury;

    return *this;
}
