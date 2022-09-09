// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayerController.h"
// APlayerPawn
#include "PlayerPawn.h"
// MousePosOnViewport
#include "Blueprint/WidgetLayoutLibrary.h"
// GameSettings
#include "GameFramework/GameUserSettings.h"
// Clamp 
#include "Math/UnrealMathUtility.h"

//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%f : %f"), CamRotation.Pitch, CamRotation.Yaw));

AMainPlayerController::AMainPlayerController()
{
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = Cast <APlayerPawn> (GetPawn());
	bEnableClickEvents = true;
	PrimaryActorTick.bCanEverTick = true;

	ZoomFactorVariation = (ZoomFactorMax - ZoomFactorMin) / ZoomScaleMin;
	ZoomScale = ZoomScaleMin;
	// Init MaxPitchAngle
	SetMaxPitchAngle();
	// Set Default SpringArm Rotation
	CameraRotation = FRotator(MaxPitchAngle, 0.0f, 0.0f);

	SetupInputComponent();
}

void AMainPlayerController::MouseEdgeScrolling (const FVector2D & Mouse, const FVector2D & Screen, float LimitX, float LimitY)
{
	const auto CheckAxis{ [&] (float MouseAxis, float ScreenSizeAxis, float LimitScreenAxis) -> void
		{
			if ((MouseAxis < (0 + LimitScreenAxis)) && !DisableCameraMovements)
			{
				float ScaleValue { FMath::Abs((FMath::Clamp(MouseAxis / LimitScreenAxis, 0.0f, 1.0f) - 1)) };
				PlayerPawn->Move((ScreenSizeAxis == Screen.X ? FVector::LeftVector : FVector::ForwardVector), ScaleValue * ZoomFactor);
			}

			if ((MouseAxis > (ScreenSizeAxis - LimitScreenAxis)) && !DisableCameraMovements)
			{
				float ScaleValue { FMath::Abs((FMath::Clamp((ScreenSizeAxis - MouseAxis) / LimitScreenAxis, 0.0f, 1.0f) - 1)) };
				PlayerPawn->Move((ScreenSizeAxis == Screen.X ? FVector::RightVector : FVector::BackwardVector), ScaleValue * ZoomFactor);
			}
		} };

	CheckAxis(Mouse.X, Screen.X, LimitX);
	CheckAxis(Mouse.Y, Screen.Y, LimitY);
}

void AMainPlayerController::AddZoom()
{
	ZoomScale = FMath::Clamp(--ZoomScale, ZoomScaleMax, ZoomScaleMin);
	ZoomFactor = FMath::Clamp(ZoomFactor -= ZoomFactorVariation, ZoomFactorMin, ZoomFactorMax);
	PlayerPawn->Zoom(ZoomScale);
	SetMaxPitchAngle();
}

void AMainPlayerController::DecreaseZoom()
{
	ZoomScale = FMath::Clamp(++ZoomScale, ZoomScaleMax, ZoomScaleMin);
	ZoomFactor = FMath::Clamp(ZoomFactor += ZoomFactorVariation, ZoomFactorMin, ZoomFactorMax);
	PlayerPawn->Zoom(ZoomScale);
	SetMaxPitchAngle();
}

void AMainPlayerController::EnableMouseCameraRotation()
{
	// To prevent mouse input to be disabled if true
	bShowMouseCursor = false;
	DisableCameraMovements = true;
	DisableCameraRotation = false;
}

void AMainPlayerController::DisableMouseCameraRotation()
{

	if (this->IsInputKeyDown(FKey("LeftAlt")) || this->IsInputKeyDown(FKey("RightMouseButton")))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s"), TEXT("TA GUEULE.")));
		return;
	}
	// To prevent mouse input to be disabled if true
	bShowMouseCursor = true;
	DisableCameraMovements = false;
	DisableCameraRotation = true;
}

void AMainPlayerController::PitchRotation(float Axis)
{
	if (!DisableCameraRotation || InputComponent->GetAxisValue("KeyboardUpRotation") != 0.0f)
		PlayerPawn->SpringArmPitchRotationByAxis(Axis, MinPitchAngle, MaxPitchAngle);
}

void AMainPlayerController::YawRotation(float Axis)
{
	if (!DisableCameraRotation || InputComponent->GetAxisValue("KeyboardRightRotation") != 0.0f)
		PlayerPawn->ArrowComponentYawRotationByAxis(Axis);
}
// DEPRECATED
void AMainPlayerController::SetManualPitchRotationByAngle(float Angle)
{
	checkf(Angle >= MinPitchAngle && Angle <= MaxPitchAngle, TEXT("Error SetManualRotationByAngle : Angle out of range."));

	// CameraRotation.Pitch = Angle;


	
	//PlayerPawn->SpringArmPitchRotationByMaxAngle(Angle);
}

void AMainPlayerController::SetMaxPitchAngle()
{
	float temp = MaxPitchAngle;
	MaxPitchAngle = MAX_PITCH_ANGLE + (((ZoomScale - ZoomScaleMax)) * (DEFAULT_PITCH_ROTATION_PAWN - MAX_PITCH_ANGLE)) / static_cast <float> (ZoomScaleMin - ZoomScaleMax);
	PlayerPawn->SpringArmPitchRotationByMaxAngle(MaxPitchAngle, temp);
}

void AMainPlayerController::MoveKeyboardForward(float Axis)
{
	if (InputComponent->GetAxisValue("KeyboardMoveForward") == 1.0f)
		PlayerPawn->Move(FVector::ForwardVector, 1);
	else if (InputComponent->GetAxisValue("KeyboardMoveForward") == -1.0f)
		PlayerPawn->Move(FVector::BackwardVector, 1);
	else
		return;

}

void AMainPlayerController::MoveKeyboardRight(float Axis)
{
	if (InputComponent->GetAxisValue("KeyboardMoveRight") == 1.0f)
		PlayerPawn->Move(FVector::RightVector, 1);
	else if (InputComponent->GetAxisValue("KeyboardMoveRight") == -1.0f)
		PlayerPawn->Move(FVector::LeftVector, 1);
	else
		return;
}

void AMainPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Setting position of the mouse at each frame
	MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(Super::GetWorld());
	// Be able to trigger Edge Scrolling at each frame
	MouseEdgeScrolling (MousePos, ScreenSize, borderX, borderY);
}

void AMainPlayerController::SetupInputComponent()
{

	Super::SetupInputComponent();
	
	if (PlayerPawn)
	{
		InputComponent->BindAction(FName("ZoomIn"), IE_Pressed, this, &AMainPlayerController::AddZoom);
		InputComponent->BindAction(FName("ZoomOut"), IE_Pressed, this, &AMainPlayerController::DecreaseZoom);
		InputComponent->BindAction(FName("CameraRotation"), IE_Pressed, this, &AMainPlayerController::EnableMouseCameraRotation);
		InputComponent->BindAction(FName("CameraRotation"), IE_Released, this, &AMainPlayerController::DisableMouseCameraRotation);
		InputComponent->BindAxis(FName("YRotation"), this, &AMainPlayerController::PitchRotation);
		InputComponent->BindAxis(FName("ZRotation"), this, &AMainPlayerController::YawRotation);
		InputComponent->BindAxis(FName("KeyboardUpRotation"), this, &AMainPlayerController::PitchRotation);
		InputComponent->BindAxis(FName("KeyboardRightRotation"), this, &AMainPlayerController::YawRotation);
		InputComponent->BindAxis(FName("KeyboardMoveForward"), this, &AMainPlayerController::MoveKeyboardForward);
		InputComponent->BindAxis(FName("KeyboardMoveRight"), this, &AMainPlayerController::MoveKeyboardRight);
	}
	
	
}

