// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LittleBigTown/Core/Structs.h"

#include "Building.generated.h"



UCLASS(Abstract)
class LITTLEBIGTOWN_API ABuilding : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuilding();

	const FBuildingInfosBase& GetInfosBase() const { return InfosBase; }

protected:

	UPROPERTY(BlueprintReadOnly)
		FBuildingInfosBase InfosBase;

	UPROPERTY(BlueprintReadOnly)
		class USceneComponent* RootComp {};

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class UStaticMeshComponent* StaticMeshComponent{};

	UPROPERTY(EditDefaultsOnly)
		FText Name;

	UPROPERTY(EditDefaultsOnly)
		int32 CurrentLevel { 1 };

	UPROPERTY(EditDefaultsOnly)
		int32 MaxLevel { 0 };

};
