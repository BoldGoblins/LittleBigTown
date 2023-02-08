// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LittleBigTown/Core/Structs.h"

#include "Building.generated.h"


// #define LOCTEXT_NAMESPACE "TempoTry"

UCLASS(Abstract)
class LITTLEBIGTOWN_API ABuilding : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuilding();

	void BeginPlay() override;

	const FBuildingInfosBase& GetInfosBase() const { return InfosBase; }

protected:

	UPROPERTY(BlueprintReadOnly)
		class USceneComponent* RootComp {};

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class UStaticMeshComponent* StaticMeshComponent{};


	UPROPERTY(EditDefaultsOnly)
		FBuildingInfosBase InfosBase;
};

// #undef LOCTEXT_NAMESPACE
