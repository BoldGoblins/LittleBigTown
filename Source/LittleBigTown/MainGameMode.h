// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Building.h"
#include "LittleBigTown.h"

#include "MainGameMode.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class LITTLEBIGTOWN_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()

public :

	/*
	UFUNCTION(BlueprintCallable)
		const TMap <FName, TSubclassOf <ABuilding>> & GetBuildingsMap() const { return BuildingsMap; }
	*/

	UFUNCTION(BlueprintCallable)
		const TMap <FName, TSubclassOf <ABuilding>>& GetBuildingsMap(const TEnumAsByte <ESlotType> & SlotType, 
			const TEnumAsByte <ESlotSize>& SlotSize) const;

protected :
	/*
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TMap <FName, TSubclassOf <ABuilding>> BuildingsMap;
		*/

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TMap <FName, TSubclassOf <ABuilding>> S_ResidentialMap;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TMap <FName, TSubclassOf <ABuilding>> M_ResidentialMap;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TMap <FName, TSubclassOf <ABuilding>> L_ResidentialMap;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TMap <FName, TSubclassOf <ABuilding>> S_CommercialMap;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TMap <FName, TSubclassOf <ABuilding>> M_CommercialMap;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TMap <FName, TSubclassOf <ABuilding>> L_CommercialMap;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TMap <FName, TSubclassOf <ABuilding>> S_OfficesMap;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TMap <FName, TSubclassOf <ABuilding>> M_OfficesMap;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TMap <FName, TSubclassOf <ABuilding>> L_OfficesMap;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TMap <FName, TSubclassOf <ABuilding>> IndustrialMap;
	
};
