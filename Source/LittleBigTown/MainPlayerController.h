// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
// For definitions
#include "LittleBigTown.h"

#include "MainPlayerController.generated.h"

UCLASS()
class LITTLEBIGTOWN_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public : 


	// --------------------------------------		GENERAL (AND HERITED) FUNCTIONS		--------------------------------------


		AMainPlayerController();
		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;
		virtual void SetupInputComponent() override;


	// --------------------------------------		PAWN CONTROL FUNCTIONS		--------------------------------------

protected : 		
		
		// Moving the camera on the (x,y) axis according to the mouse position on the screen.
		// Call move function of Pawn class
		void MouseEdgeScrolling ();

		// Zooming in and out by axis
		// Compute ZoomScale, ZoomFactor and RequiredZLocation
		// Call Zoom and SetRequiredZLocation in Pawn class and SetMaxPitchAngle function in Player Controller class
		void Zoom(float Axis);

		// Enable/Disable Camera Rotation when Axis button is pressed
		// Modify InputMode, MouseCursor, DisableMovement and DisableRotation
		void EnableMouseCameraRotation();
		// Check that there is no other Rotation related keys that are still pressed before making changes in variables
		void DisableMouseCameraRotation();

		// Scan mouse XY and get Pitch Rotation
		// This will be used by Pawn to compute Spring Arm rotation
		// Call SpringArmPitchRotationByAxis in Pawn class
		void PitchRotation(float Axis);

		// Scan mouse XY and get Pitch Rotation
		// This will be used by Pawn to compute Arrow rotation
		// Call ArrowComponentYawRotationByAxis in Pawn class
		void YawRotation(float Axis);

		// Compute new MaxPitchAngle value based on current ZoomScale
		// Pawn will use this value to automaticaly set the new Pitch rotation for the Spring Arm
		// (if Spring Arm rotation is close to new MaxPitchAngle)
		void SetMaxPitchAngle();

		// Keyboard controls for input to Pawn Movement
		void MoveKeyboardForward(float Axis);
		void MoveKeyboardRight(float Axis);


private : 
		// Check on axis on by one to determine in where direction and how much move the pawn
		void CheckMouseAxisForEdgeScrolling(float MouseAxis, float ScreenSizeAxis, float LimitScreenAxis);


	// --------------------------------------		PAWN CONTROL PARAMETERS		--------------------------------------


protected :

	// Hold Reference to the current PlayerPawn
	UPROPERTY(BlueprintReadOnly)
		class APlayerPawn* PlayerPawn {};

	// Set this value to true to disable all kinds of inputs to the Pawn class (zoom, move, rotate)
	UPROPERTY(BlueprintReadWrite, Transient, Category = "Pawn Control Parameter")
		bool DisablePawnControl { false };

	// Set the input mode to game in rotation (to avoid mouse capture issues)
	FInputModeGameOnly InputModeGameOnly {};

	// Set the input mode to game and ui otherwise (maybe just UI ?)
	FInputModeGameAndUI InputModeGameAndUI {};


	// --------------------------------------		SCREEN PARAMETERS		--------------------------------------
	

	// Mouse Position on the viewport, updated in C++
	UPROPERTY(BlueprintReadWrite, Category = "Screen Parameters")
		FVector2D MousePos {};

	// Value currently Edited in BP
	UPROPERTY(BlueprintReadWrite, Transient)
		FVector2D ScreenSize;

	// Border (10% of ScreenSize.X) where to trigger Pawn movement. Edit via BP method.
	UPROPERTY(BlueprintReadWrite, Transient)
		float borderX;

	// Border (10% of ScreenSize.Y) where to trigger Pawn movement. Edit via BP method.
	UPROPERTY(BlueprintReadWrite, Transient)
		float borderY;


	// --------------------------------------		ZOOM PARAMETERS		  --------------------------------------
	
	
	// Compute the length of SpringArm, by default == to ZoomScaleMin
	UPROPERTY(BlueprintReadOnly, Category = "Zoom Parameters")
		int ZoomScale;

	// Default min clamped value for ZoomScale
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Zoom Parameters")
		int ZoomScaleMin { ZOOM_MIN };

	// Default max clamped value for ZoomScale
	UPROPERTY(BlueprintReadOnly, Category = "Zoom Parameters")
		int ZoomScaleMax { ZOOM_MAX };

	// Zoom Factor used to compute edge scrolling scaling (move faster as we zoom out and slower as we zoom in)
	UPROPERTY(BlueprintReadOnly, Category = "Zoom Parameters")
		float ZoomFactor { ZOOM_FACTOR_MAX };

	// Default min clamped value for ZoomFactor, can be set manually to increase deceleration of edge scrolling as much as ZoomScale is close to ZoomMax
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Zoom Parameters")
		float ZoomFactorMin { ZOOM_FACTOR_MIN };

	// Default max clamped value for ZoomFactor, always equal to 1
	UPROPERTY(BlueprintReadOnly, Category = "Zoom Parameters")
		float ZoomFactorMax { ZOOM_FACTOR_MAX };

	// Variation (increase and decrease of ZoomFactor between min and max)
	UPROPERTY(BlueprintReadOnly, Category = "Zoom Parameters")
		float ZoomFactorVariation;


	// --------------------------------------		CAMERA PARAMETERS		  --------------------------------------


	// Disable movements of the camera when set to true.
	UPROPERTY(BlueprintReadWrite, Category = "Camera Parameters")
		bool DisableCameraMovements {false};
	// Disable rotation of the camera when set to true.
	UPROPERTY(BlueprintReadWrite, Category = "Camera Parameters")
		bool DisableCameraRotation { true };

	// Cannot be > - 80 because it's unstable otherwise
	UPROPERTY(BlueprintReadOnly, Category = "Camera Parameters")
		float MinPitchAngle { MIN_PITCH_ANGLE };

	// MaxPitchAngle is set by SetMaxPitchAngle automaticaly
	UPROPERTY(BlueprintReadOnly, Category = "Camera Parameters")
		float MaxPitchAngle;


	// --------------------------------------		PAWN COLLISIONS PARAMETERS		  --------------------------------------


	// Minimum altitude possible for the pawn 
	// Must be > 50 at least (because pawn distance calculation is underestimated by 50)
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Pawn collisions parameters")
		float PawnMinLocationZAxis{ DEFAULT_PAWN_LOCATION_Z_AXIS_MIN };

	// Maximum altitude possible for the pawn
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Pawn collisions parameters")
		float PawnMaxLocationZAxis{ DEFAULT_PAWN_LOCATION_Z_AXIS_MAX };

};
