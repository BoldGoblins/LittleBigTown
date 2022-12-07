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
		const TMap <FName, FBuildingContainers> & GetBuildingsMap(TEnumAsByte <ESlotType> SlotType,
			TEnumAsByte <ESlotSize> SlotSize) const;

protected :
	/*
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TMap <FName, TSubclassOf <ABuilding>> BuildingsMap;
		*/

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TMap <FName, FBuildingContainers> S_ResidentialMap;


	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TMap <FName, FBuildingContainers> M_ResidentialMap;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TMap <FName, FBuildingContainers> L_ResidentialMap;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TMap <FName, FBuildingContainers> S_CommercialMap;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TMap <FName, FBuildingContainers> M_CommercialMap;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TMap <FName, FBuildingContainers> L_CommercialMap;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TMap <FName, FBuildingContainers> S_OfficesMap;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TMap <FName, FBuildingContainers> M_OfficesMap;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TMap <FName, FBuildingContainers> L_OfficesMap;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TMap <FName, FBuildingContainers> IndustrialMap;
	
};
