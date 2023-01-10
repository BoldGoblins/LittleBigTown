// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Pawn.h"
// Player Controller header
#include "MainPlayerController.h"

#include "PlayerPawn.generated.h"

UCLASS(Abstract)
class LITTLEBIGTOWN_API APlayerPawn : public APawn
{
	GENERATED_BODY()


	// --------------------------------------		GENERAL (AND HERITED) FUNCTIONS		--------------------------------------


public:
	// Sets default values for this pawn's properties
	APlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// --------------------------------------		PAWN CONTROL FUNCTIONS		--------------------------------------


	// Move along a direction given by World, at a certain scale
	// Function called by MouseEdgeScrolling (Player Controller)
	// Call AddMovementInput function itself
	void Move(const FVector& World, float scale);

	// Called by Zoom, updates CamZoomDestination, so movement can be triggered via Tick statement (Interep)
	// Call TryToMovePawnAtRequiredZLocation
	void Zoom(float ZoomScale);
	
	// Add a given Angle to SpringArm Pitch actual rotation
	// Clamp new Pitch Rotation between min and max Pitch Angle
	void AddSpringArmPitchRotation (float Angle, float MinPitchAngle, float MaxPitchAngle);

	// Set a new given Angle to SpringArmPitch after clamped it betwen min and max pitch angle
	void SetSpringArmPitchRotation(float NewAngle, float MinPitchAngle, float MaxPitchAngle);

	// Return SpringArmPitchRotation
	float GetSpringArmPitchRotation();

	// Add Rotation to the ArrowComponent by a given Angle
	void AddArrowComponentYawRotation (float Angle);

	// SetNewRequiredZLocation for the pawn (computed from ZoomScale in Zoom function)
	void SetRequiredZLocation(float NewZLocation) { RequiredZLocation = NewZLocation; }


private:

	// Check in 8 direction from the Pawn for collisions
	bool CollisionQueryAlongXYAxis();

	// Return Distance available under Pawn (-50, to allow a certain distance between pawn and others objects)
	float FindAvailableDistanceUnderPawn();

	// Called from Zoom and Move function 
	// Continuously try to adjust to the Required Z Location
	// Call FindAvailableDistanceUnderPawn before adjusting
	// If there is not enough distance under pawn, ZLocation is set as closest as possible of RequiredZLocation
	void TryMovePawnAtRequiredZLocation();


protected :


	// --------------------------------------		PAWN COMPONENTS		  --------------------------------------


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class UCameraComponent* CameraComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class USphereComponent* SphereComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class UFloatingPawnMovement* PawnMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class UArrowComponent* ArrowComp;


	// --------------------------------------		ZOOM PARAMETERS		  --------------------------------------

	// How much to multiply to forward vector of SpringArmComp
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Zoom Parameters")
		int ZoomUnits { DEFAULT_ZOOM_UNITS };


	// --------------------------------------		REFERENCES PARAMETERS		  --------------------------------------


	UPROPERTY(BlueprintReadOnly)
		class AMainPlayerController* PlayerController;

	// --------------------------------------		PAWN COLLISION PARAMETERS		  --------------------------------------


	// RequiredZLocation (Depend on ZoomScale)
	UPROPERTY(BlueprintReadOnly, Category = "Pawn collisions parameters")
		float RequiredZLocation{};


	// --------------------------------------		IMPLEMENTATION PARAMETERS		  --------------------------------------
	

};
