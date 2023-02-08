// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

// For definitions
#include "LittleBigTown/LittleBigTown.h"
#include "LittleBigTown/GameSystem/MainGameState.h"

#include "MainPlayerController.generated.h"

#define ZOOM_MIN 10
#define ZOOM_MAX 1

#define ZOOM_FACTOR_MIN 0.800000f
#define ZOOM_FACTOR_MAX 1.000000f 

#define DEFAULT_ZOOM_UNITS 500

// Remember that MaxSpeed/Acceleration = time to get to MaxSpeed
// Exemple : 2 / 200 = 0.01 sec to get to Speed = 2

#define MAX_SPEED 25000,0
#define ACCELERATION 250000,0
#define DECELERATION 250000,0

#define DEFAULT_SPRING_ARM_LENGTH 6000.000000f

#define MIN_PITCH_ANGLE  - 80.0f
#define MAX_PITCH_ANGLE  - 10.0f 

#define DEFAULT_PITCH_ROTATION_PAWN - 50.0f

#define DEFAULT_PAWN_LOCATION_Z_AXIS_MIN 300.0f
#define DEFAULT_PAWN_LOCATION_Z_AXIS_MAX 1800.0f

#define MAX_ALTITUDE_IN_LEVEL 10000


UCLASS(Abstract)
class LITTLEBIGTOWN_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public : 


	// --------------------------------------		GENERAL (AND HERITED) FUNCTIONS		--------------------------------------


		AMainPlayerController();
		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;
		virtual void SetupInputComponent() override;
		
		UFUNCTION(BlueprintCallable)
			const AMainGameMode* GetMainGameMode() { return GameMode; }

		// Peut être à supprimer si pas besoin (ne pourra pas être répliqué en multi...)
		UFUNCTION(BlueprintCallable)
			const AMainGameState* GetGameState() { return GameState; }

	// --------------------------------------		WIDGETS FUNCTIONS		  --------------------------------------


		// WILL BE DEPRECATED IN FUTURE : UUI_Main'll handle Widgets management EDIT : Maybe not
		UFUNCTION(BlueprintCallable)
			class UUI_ConstructionMain* GetConstructionWidget();

		UFUNCTION(BlueprintCallable)
			class UUI_Main* GetMainWidget();

		UFUNCTION(BlueprintCallable)
			class UUI_GraphsMain* GetGraphsWidget() const { return GraphsWidget; }

		class UUI_BuildingInfos* GetBuildingInfos() const;

		void SetMainWidget(class UUI_Main* Widget);
		void SetBuildingInfosWidget(class UUI_BuildingInfos* Widget);
		void SetGraphWidget(class UUI_GraphsMain* Widget);

		// WILL BE DEPRECATED IN FUTURE : UUI_Main'll handle Widgets management EDIT : Maybe not
		// Called in ThematicUI_Template NativeConstruct
		void SetConstructionWidget(class UUI_ConstructionMain* Widget);


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


protected :


	// --------------------------------------		GENERAL PARAMETERS		--------------------------------------


	UPROPERTY(BlueprintReadOnly)
		class UWorld* World{};


	// --------------------------------------		PAWN CONTROL PARAMETERS		--------------------------------------
	 
	
	// Hold Reference to the current PlayerPawn
	UPROPERTY(BlueprintReadOnly)
		class APlayerPawn* PlayerPawn {};

	UPROPERTY(BlueprintReadOnly)
		class AMainGameMode* GameMode {};

	UPROPERTY(BlueprintReadOnly)
		class AMainGameState* GameState{};

	// Set this value to true to disable all kinds of inputs to the Pawn class (zoom, move, rotate)
	UPROPERTY(BlueprintReadWrite, Transient, Category = "Pawn Control Parameter")
		bool DisablePawnControl { false };

	// Set the input mode to game in rotation (to avoid mouse capture issues)
	FInputModeGameOnly InputModeGameOnly {};

	// Set the input mode to game and ui otherwise (maybe just UI ?)
	FInputModeGameAndUI InputModeGameAndUI {};


	// --------------------------------------		MOUSE PARAMETERS		--------------------------------------
	

	// Mouse Position on the viewport, updated in C++
	UPROPERTY(BlueprintReadWrite, Category = "Screen Parameters")
		FVector2D MousePos {};


	// --------------------------------------		SCREEN PARAMETERS		--------------------------------------


	// Value currently Edited in BP
	// Set in ReadOnly ??
	UPROPERTY(BlueprintReadWrite, Transient)
		FVector2D ScreenSize;


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


	// --------------------------------------		WIDGETS PARAMETERS		  --------------------------------------

	// WILL BE DEPRECATED IN FUTURE : UUI_Main'll handle Widgets management
	UPROPERTY(BlueprintReadOnly)
		class UUI_ConstructionMain* ConstructionWidget {nullptr};

	UPROPERTY(BlueprintReadOnly)
		class UUI_Main* MainWidget { nullptr };

	UPROPERTY(BlueprintReadOnly)
		class UUI_BuildingInfos* BuildingInfosWidget { nullptr };

	UPROPERTY(BlueprintReadOnly)
		class UUI_GraphsMain* GraphsWidget { nullptr };
};
