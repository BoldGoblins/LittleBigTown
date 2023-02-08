// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LittleBigTown/Actors/Building.h"
#include "LittleBigTown/Core/Enums.h"
#include "LittleBigTown/Core/Structs.h"
#include "LittleBigTown/Core/Resident.h"
#include "LittleBigTown/Core/SocialClass.h"

#include "MainGameMode.generated.h"


UCLASS(Abstract)
class LITTLEBIGTOWN_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()

public :

	void BeginPlay() override;

	// Used by BuildingSelectionWidget and BP_MainPlayerController to populate ScrollBox and Spawn Buildings
	UFUNCTION(BlueprintCallable)
		const TMap <FName, FBuildingContainers> & GetBuildingsMap(TEnumAsByte <ESlotType> SlotType,
			TEnumAsByte <ESlotSize> SlotSize) const;

	const TArray <FText>& GetSpecialtyNames(const TEnumAsByte<enum EWealthLevels>& Wealth) const;

protected :

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FClassNames ClassNames;


	// All maps for buildings

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
