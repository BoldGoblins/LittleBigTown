// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Pedestrian.generated.h"

UCLASS()
class LITTLEBIGTOWN_API APedestrian : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	// void Init (struct FResident* Resident, class AResidentialBuilding* Origin, bool bIsMale);

	void Init(class AResidentialBuilding* Origin, bool bIsMale);

	// UFUNCTION(BlueprintCallable)
		// void Init(class AResidentialBuilding* Origin, bool bIsMale);

	const TWeakObjectPtr <class AResidentialBuilding>& GetOrigin() const { return _Origin; }

	// struct FResident* GetResident() { return _Resident; }

	APedestrian();

private:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FText _Name;

	// Safe to access because if we Remove the Resident from Array in ResBuilding,
	// It automaticaly tries to Destroy the Associated Pawn
	// struct FResident* _Resident;

	UPROPERTY()
		TWeakObjectPtr <class AResidentialBuilding> _Origin;

	UPROPERTY(EditDefaultsOnly)
		class USkeletalMeshComponent* _SkeletalMeshComp{};

	UPROPERTY()
		class UArrowComponent* _ArrowComp{};

	UPROPERTY(EditDefaultsOnly)
		class USkeletalMesh* MaleSkeletal {};
	
	UPROPERTY(EditDefaultsOnly)
		class USkeletalMesh* FemaleSkeletal {};

	UPROPERTY()
		class UFloatingPawnMovement* _PawnMovComp {};

	// class UCapsuleComponent* _CapsuleComp {};


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	// virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
