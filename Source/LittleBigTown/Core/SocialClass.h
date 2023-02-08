// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LittleBigTown/Core/Structs.h"

#include "SocialClass.generated.h"

USTRUCT(BlueprintType)
struct FSocialClasses
{
	GENERATED_USTRUCT_BODY()

	const TMap <FName, FNeeds>& GetMap (const TEnumAsByte<ECitySpecialty>& CitySpecialty) const;

	FName GetRandomSubClass(const TEnumAsByte<ECitySpecialty>& CitySpecialty) const;

	UPROPERTY(EditDefaultsOnly)
		TMap <FName, FNeeds> m_Industry{};

	UPROPERTY(EditDefaultsOnly)
		TMap <FName, FNeeds> m_Finance{};

	UPROPERTY(EditDefaultsOnly)
		TMap <FName, FNeeds> m_Science{};

	UPROPERTY(EditDefaultsOnly)
		TMap <FName, FNeeds> m_Tourism{};

	UPROPERTY(EditDefaultsOnly)
		TMap <FName, FNeeds> m_Crime{};

	UPROPERTY(EditDefaultsOnly)
		TMap <FName, FNeeds> m_Military{};

	UPROPERTY(EditDefaultsOnly)
		TMap <FName, FNeeds> m_Spiritual{};

};

USTRUCT(BlueprintType)
struct FClassNames
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere)
		TArray <FText> m_PoorNames{};

	UPROPERTY(EditAnywhere)
		TArray <FText> m_MiddleNames{};

	UPROPERTY(EditAnywhere)
		TArray <FText> m_RichNames{};
};
