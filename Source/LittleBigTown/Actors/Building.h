// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LittleBigTown/Core/Structs.h"
#include "LittleBigTown/Core/Resident.h"

#include "Building.generated.h"


UCLASS(Abstract)
class LITTLEBIGTOWN_API ABuilding : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuilding();

	void BeginPlay() override;

	const FBuildingInfosBase& GetInfosBase() const { return InfosBase; }

	const TArray <FResident>& GetAllOccupants() const { return m_Occupants; }

	const TArray <FResident> GetOccupants(const ECitySpecialty& Specialty) const;

	const TArray <FResident> GetOccupants(const FString & SubClassName) const;

	UFUNCTION(BlueprintCallable)
		void SetDistrictName(const FName& DistrictName) { InfosBase.m_DistrictName = FText::FromString(DistrictName.ToString()); }

protected:

	UPROPERTY(BlueprintReadOnly)
		class USceneComponent* RootComp {};

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class UStaticMeshComponent* StaticMeshComponent{};

	UPROPERTY(EditDefaultsOnly)
		FBuildingInfosBase InfosBase;

	TArray <FResident> m_Occupants{};
};
