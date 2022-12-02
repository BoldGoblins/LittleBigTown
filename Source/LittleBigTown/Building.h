// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Building.generated.h"

UCLASS()
class LITTLEBIGTOWN_API ABuilding : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuilding();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
		class USceneComponent* RootComp {};

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class UStaticMeshComponent* StaticMeshComponent{};

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
