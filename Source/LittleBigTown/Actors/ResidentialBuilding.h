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
// #define OCCUPATION_VARIATION_FACTOR 1.5f

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

	void GetSpawnableResident(FResident* ResidentPtr);

	const FVector& GetSpawnTarget() const { return _SpawnTarget; }

	void TimerSpawnPedestrian();

	// const TArray <FResident>& GetResidents() const { return m_Residents; }

	FOnResBuildingInfosChangedSignature OnResBuildingInfosChangedDelegate;

	void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite)
		int32 _DistancePedestrianSpawn { 500 };

protected : 

	UPROPERTY(BlueprintReadOnly)
		class AMainGameState* MainGameState;

	UPROPERTY(EditAnywhere)
		FResidentialBuildingInfos ResidentialInformations;

	FTimerHandle SpawnTimerHandle {};

	UPROPERTY(BlueprintReadOnly)
		FVector _SpawnTarget {};

	//  <FResident> m_Residents {};


private : 

	void GenerateResidents(int32 Count);

	void RemoveResidents(int32 Count);

	// Create Rand Table pour choisir une Spécialité au hasard (pondération selon % de demande) 
	// Doit être appelé dans GenerateResident()
	void CreateRandTable(TArray <int32> & Arr, const TMap<ECitySpecialty, float> & Freq);

	// Permet d'ajouter un FResident dans m_Occupation en le plaçant à l'Index permettant de maintenir un ordre croissant de Satisfaction
	// Ce classement est valable si et seul. si la Satisfaction de base (à l'instanciation) pour un FResident == 0.5f
	void EmplaceResident(const TEnumAsByte <ECitySpecialty>& Specialty);

// 	int32 _ResidentSpawnIndex { 0 };


};
