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

	PlayerPawn->SetRequiredZLocation(PawnMaxLocationZAxis);

	SetupInputComponent();
}

void AMainPlayerController::MouseEdgeScrolling ()
{
	if (DisablePawnControl)
		return;

	CheckMouseAxisForEdgeScrolling(MousePos.X, ScreenSize.X, borderX);
	CheckMouseAxisForEdgeScrolling(MousePos.Y, ScreenSize.Y, borderY);
}

void AMainPlayerController::Zoom(float Axis)
{
	if (DisablePawnControl)
		return;

	if (Axis > 0)
	{
		if (ZoomScale == ZoomScaleMax)
			return;

		--ZoomScale;
		ZoomFactor = FMath::Clamp(ZoomFactor -= ZoomFactorVariation, ZoomFactorMin, ZoomFactorMax);
	}
	else if (Axis < 0)
	{
		if (ZoomScale == ZoomScaleMin)
			return;

		++ZoomScale;
		ZoomFactor = FMath::Clamp(ZoomFactor += ZoomFactorVariation, ZoomFactorMin, ZoomFactorMax);
	}
	else
		return;
	// Set RequiredZLocation of the Pawn from ZoomScale
	float NewRequiredZLocation{ PawnMinLocationZAxis + (((ZoomScale - ZoomScaleMax)) * (PawnMaxLocationZAxis - PawnMinLocationZAxis) / static_cast <float> (ZoomScaleMin - ZoomScaleMax)) };

	PlayerPawn->SetRequiredZLocation(NewRequiredZLocation);
	PlayerPawn->Zoom(ZoomScale);

	SetMaxPitchAngle();
}

void AMainPlayerController::EnableMouseCameraRotation()
{
	if (DisablePawnControl)
		return;

	this->SetInputMode(InputModeGameOnly);
	bShowMouseCursor = false;
	DisableCameraMovements = true;
	DisableCameraRotation = false;
}

void AMainPlayerController::DisableMouseCameraRotation()
{
	// Don't disable rotation if another rotation action key is still pressed down
	if (this->IsInputKeyDown(FKey("LeftAlt")) || this->IsInputKeyDown(FKey("RightMouseButton")))
		return;

	this->SetInputMode(InputModeGameAndUI);
	// To prevent mouse input to be disabled if true
	bShowMouseCursor = true;
	DisableCameraMovements = false;
	DisableCameraRotation = true;
}

void AMainPlayerController::PitchRotation(float Axis)
{
	if (DisablePawnControl)
		return;

	if (!DisableCameraRotation || InputComponent->GetAxisValue("KeyboardUpRotation") != 0.0f)
		PlayerPawn->AddSpringArmPitchRotation(Axis, MinPitchAngle, MaxPitchAngle);
}

void AMainPlayerController::YawRotation(float Axis)
{
	if (DisablePawnControl)
		return;

	if (!DisableCameraRotation || InputComponent->GetAxisValue("KeyboardRightRotation") != 0.0f)
		PlayerPawn->AddArrowComponentYawRotation(Axis);
}

void AMainPlayerController::SetMaxPitchAngle()
{
	// Save value of previous MaxPitchAngle before assigning a new one
	float temp = MaxPitchAngle;

	MaxPitchAngle = MAX_PITCH_ANGLE + (((ZoomScale - ZoomScaleMax)) * (DEFAULT_PITCH_ROTATION_PAWN - MAX_PITCH_ANGLE)) / static_cast <float> (ZoomScaleMin - ZoomScaleMax);

	FRotator SpringArmRotation { PlayerPawn->GetSpringArmPitchRotation() };
	
	// Compare current SpringArm rotation and previous MaxPitchAngle
	// If it's nearly equal, set the new MaxPitchAngle as the new Pitch Rotation of SpringArm
	if (FMath::IsNearlyEqual(SpringArmRotation.Pitch, temp, 0.5f))
		PlayerPawn->SetSpringArmPitchRotation(MaxPitchAngle, MinPitchAngle, MaxPitchAngle);
}

void AMainPlayerController::MoveKeyboardForward(float Axis)
{
	if (DisablePawnControl)
		return;

	if (InputComponent->GetAxisValue("KeyboardMoveForward") == 1.0f)
		PlayerPawn->Move(FVector::ForwardVector, 1);
	else if (InputComponent->GetAxisValue("KeyboardMoveForward") == -1.0f)
		PlayerPawn->Move(FVector::BackwardVector, 1);
	else
		return;

}

void AMainPlayerController::MoveKeyboardRight(float Axis)
{
	if (DisablePawnControl)
		return;

	if (InputComponent->GetAxisValue("KeyboardMoveRight") == 1.0f)
		PlayerPawn->Move(FVector::RightVector, 1);
	else if (InputComponent->GetAxisValue("KeyboardMoveRight") == -1.0f)
		PlayerPawn->Move(FVector::LeftVector, 1);
	else
		return;
}

void AMainPlayerController::CheckMouseAxisForEdgeScrolling(float MouseAxis, float ScreenSizeAxis, float LimitScreenAxis)
{
	if ((MouseAxis < (0 + LimitScreenAxis)) && !DisableCameraMovements)
	{
		float ScaleValue{ FMath::Abs((FMath::Clamp(MouseAxis / LimitScreenAxis, 0.0f, 1.0f) - 1)) };
		PlayerPawn->Move((ScreenSizeAxis == ScreenSize.X ? FVector::LeftVector : FVector::ForwardVector), ScaleValue * ZoomFactor);
	}

	if ((MouseAxis > (ScreenSizeAxis - LimitScreenAxis)) && !DisableCameraMovements)
	{
		float ScaleValue{ FMath::Abs((FMath::Clamp((ScreenSizeAxis - MouseAxis) / LimitScreenAxis, 0.0f, 1.0f) - 1)) };
		PlayerPawn->Move((ScreenSizeAxis == ScreenSize.X ? FVector::RightVector : FVector::BackwardVector), ScaleValue * ZoomFactor);
	}
}

void AMainPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Setting position of the mouse at each frame
	MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(Super::GetWorld());
	// Be able to trigger Edge Scrolling at each frame
	MouseEdgeScrolling ();
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (PlayerPawn)
	{
		InputComponent->BindAction(FName("CameraRotation"), IE_Pressed, this, &AMainPlayerController::EnableMouseCameraRotation);
		InputComponent->BindAction(FName("CameraRotation"), IE_Released, this, &AMainPlayerController::DisableMouseCameraRotation);
		InputComponent->BindAxis(FName("YRotation"), this, &AMainPlayerController::PitchRotation);
		InputComponent->BindAxis(FName("ZRotation"), this, &AMainPlayerController::YawRotation);
		InputComponent->BindAxis(FName("KeyboardUpRotation"), this, &AMainPlayerController::PitchRotation);
		InputComponent->BindAxis(FName("KeyboardRightRotation"), this, &AMainPlayerController::YawRotation);
		InputComponent->BindAxis(FName("KeyboardMoveForward"), this, &AMainPlayerController::MoveKeyboardForward);
		InputComponent->BindAxis(FName("KeyboardMoveRight"), this, &AMainPlayerController::MoveKeyboardRight);
		InputComponent->BindAxis(FName("Zoom"), this, &AMainPlayerController::Zoom);
	}
}

