// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Pawn.h"
// For definitions
#include "LittleBigTown.h"

#include "PlayerPawn.generated.h"

UCLASS()
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


	// Called by AddZoom or DecreaseZoom, updates CamZoomDestination, so movement can be triggered via Tick statement (Interep)
	// Assume scale is already clamped (1-10) before coming as argument
	void Zoom(int scale);
	
	void SpringArmPitchRotation (const FRotator& Rotation);


protected :


	// --------------------------------------		PAWN COMPONENTS		  --------------------------------------


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class UCameraComponent* CameraComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* SphereComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class UFloatingPawnMovement* PawnMovement;


	// --------------------------------------		ZOOM PARAMETERS		  --------------------------------------


	// Speed of the Interp that allows camera to move smoothly
	UPROPERTY(BlueprintReadOnly, Category = "Zoom Parameters")
		float ZoomInterpSpeed;

	// How much resize the Spring Arm at each Zoom (in/out)
	UPROPERTY(BlueprintReadOnly, Category = "Zoom Parameters")
		int ZoomUnits;


	// --------------------------------------		REFERENCES PARAMETERS		  --------------------------------------


	UPROPERTY(BlueprintReadOnly)
		class AMainPlayerController* PlayerController;


	// --------------------------------------		IMPLEMENTATION PARAMETERS		  --------------------------------------
	

	// Use to store the update resized value of the spring arm so it's can be used as a target by the Interp function
	float CamZoomDestination { DEFAULT_SPRING_ARM_LENGTH };
};
