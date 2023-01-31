// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Building.h"
// struct residential infos definition
#include "LittleBigTown/Core/Structs.h"
// Enum WealthLevels definition
#include "LittleBigTown/Core/Enums.h"
#include "LittleBigTown/Core/Resident.h"

#include "ResidentialBuilding.generated.h"

#define MAX_LEVEL_RESIDENTIAL 5
#define TEN_PERCENT_OCCUPATION FMath::RoundToInt ((InfosBase.m_OccupationMaxCount * 10) / 100)
#define OCCUPATION_VARIATION_FACTOR 1.5f
#define BASE_INCOMES 50
#define BASE_SATISFACTION 1.0f

// Besoin pour communiquer avec le UI_BuildingInfos
// Celui-ci à déjà accès au Building currently displayed via un WeakObjectPtr (lui permet de Unbind la delegate)
// Du coup, le widget n'a pas besoin des infos passées en paramètre de la delegate...
// Si pas d'autres communications requises, considérer d'enlever les paramètres et de garder une delegate sans params...
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResBuildingInfosChangedSignature);

UCLASS()
class LITTLEBIGTOWN_API AResidentialBuilding : public ABuilding
{
	GENERATED_BODY()

public :

	UFUNCTION()
		void UpdateNewHour(int32 Hour);

	UFUNCTION(BlueprintCallable)
		void AddSatisfaction(float Satisfaction);

	UFUNCTION(BlueprintCallable)
		const FResidentialBuildingInfos & GetInfosResidential() const { return ResidentialInformations; }

	FOnResBuildingInfosChangedSignature OnResBuildingInfosChangedDelegate;

	void BeginPlay() override;

protected : 

	UPROPERTY(BlueprintReadOnly)
		class AMainGameState* MainGameState;

	UPROPERTY(EditDefaultsOnly)
		int32 IncomePerHabitant { BASE_INCOMES };


	FResidentialBuildingInfos ResidentialInformations;

	TArray <FResident> m_Residents {};


private : 

	void GenerateResidents(int32 Count);


};
