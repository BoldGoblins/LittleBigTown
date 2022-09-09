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
		
		
		// Normalement pas besoin de check les FPS, à vérifier /!\
		// Moving the camera on the (x,y) axis according to the mouse position on the screen.
		// Input : Position of the mouse, size of the viewport, limits (X,Y) from where to start moving camera (use BorderX&Y values)
		// Call move function of Pawn class
		void MouseEdgeScrolling (const FVector2D& Pos, const FVector2D& Size, float LimitX, float LimitY);

		// Bind to action mapping for zoom out and zoom in 
		// Update ZoomScale and ZoomFactor in Player Controller
		// Call Zoom in Player Pawn
		void AddZoom();
		void DecreaseZoom();

		void EnableMouseCameraRotation();
		void DisableMouseCameraRotation();

		void PitchRotation(float Axis);

		void YawRotation(float Axis);
		// To compute ZoomUnits and ZoomInterpSpeed in Pawn
		int GetZoomMin() { return ZoomScaleMin; }

		// DEPRECATED
		void SetManualPitchRotationByAngle(float Angle);

		void SetMaxPitchAngle();

		void MoveKeyboardForward(float Axis);

		void MoveKeyboardRight(float Axis);


protected :

	// Hold Reference to the current PlayerPawn
	UPROPERTY(BlueprintReadOnly)
		class APlayerPawn* PlayerPawn {};

	// Mouse Position on the viewport, updated in C++
	UPROPERTY(BlueprintReadOnly)
		FVector2D MousePos {};


	// --------------------------------------		SCREEN PARAMETERS		--------------------------------------
	

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

	// Default min clamped value for ZoomFactor
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Zoom Parameters")
		float ZoomFactorMin { ZOOM_FACTOR_MIN };

	// Default max clamped value for ZoomFactor
	UPROPERTY(BlueprintReadOnly, Category = "Zoom Parameters")
		float ZoomFactorMax { ZOOM_FACTOR_MAX };

	// Variation (increase and decrease of ZoomFactor between min and max)
	UPROPERTY(BlueprintReadOnly, Category = "Zoom Parameters")
		float ZoomFactorVariation;


	// --------------------------------------		CAMERA PARAMETERS		  --------------------------------------


	// Disable movements of the camera when set to true.
	UPROPERTY(BlueprintReadWrite, Category = "Camera Parameters")
		bool DisableCameraMovements {false};

	UPROPERTY(BlueprintReadWrite, Category = "Camera Parameters")
		bool DisableCameraRotation { true };

	// DEPRECATED
	UPROPERTY(BlueprintReadOnly)
		FRotator CameraRotation;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Camera Parameters")
		float MinPitchAngle { MIN_PITCH_ANGLE };

	UPROPERTY(BlueprintReadOnly, Category = "Camera Parameters")
		float MaxPitchAngle;
};
