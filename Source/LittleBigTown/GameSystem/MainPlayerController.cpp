// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayerController.h"
// APlayerPawn
#include "PlayerPawn.h"
#include "MainGameMode.h"
#include "LittleBigTown/UserInterface/Construction/UI_ConstructionMain.h"
// MousePosOnViewport
#include "Blueprint/WidgetLayoutLibrary.h"
// GameSettings
#include "GameFramework/GameUserSettings.h"
// Clamp 
#include "Math/UnrealMathUtility.h"
// UI_Main
#include "LittleBigTown/UserInterface/Main/UI_Main.h"
// BuildingInfos
#include "LittleBigTown/UserInterface/BuildingInfos/UI_BuildingInfos.h"
// Graphs Main
#include "LittleBigTown/UserInterface/Graphs/UI_GraphsMain.h"
// DEBUG_ONLY
#include "LittleBigTown/Core/Debugger.h"


//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%f : %f"), CamRotation.Pitch, CamRotation.Yaw));

#define SCALE_PAWN_MOVEMENT (ZoomFactor * 250) * (GetWorld()->DeltaTimeSeconds * 60)

AMainPlayerController::AMainPlayerController()
{
	InputModeGameAndUI.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	World = Super::GetWorld();

	PlayerPawn = Cast <APlayerPawn> (GetPawn());
	GameMode = Cast <AMainGameMode> (World->GetAuthGameMode());
	GameState = Cast <AMainGameState>(World->GetGameState());


	bEnableClickEvents = true;
	PrimaryActorTick.bCanEverTick = true;

	ZoomFactorVariation = (ZoomFactorMax - ZoomFactorMin) / ZoomScaleMin;
	ZoomScale = ZoomScaleMin;

	this->SetInputMode(InputModeGameAndUI);

	// Init MaxPitchAngle
	SetMaxPitchAngle();

	PlayerPawn->SetRequiredZLocation(PawnMaxLocationZAxis);

	SetupInputComponent();
}

void AMainPlayerController::MouseEdgeScrolling ()
{
	if (DisablePawnControl)
		return;

	// Cap Scale pour qu'il ne dépende pas des FPS
	float Scale { SCALE_PAWN_MOVEMENT };
	/*
	if (FMath::IsNearlyEqual(MousePos.X, 0, 10.0f) && !DisableCameraMovements)
		PlayerPawn->Move(FVector::LeftVector, Scale);
	
	else if (FMath::IsNearlyEqual(MousePos.X, ScreenSize.X, 10.0f) && !DisableCameraMovements)
		PlayerPawn->Move(FVector::RightVector, Scale);

	if (FMath::IsNearlyEqual(MousePos.Y, 0, 10.0f) && !DisableCameraMovements)
		PlayerPawn->Move(FVector::ForwardVector, Scale);

	else if (FMath::IsNearlyEqual(MousePos.Y, ScreenSize.Y, 10.0f) && !DisableCameraMovements)
		PlayerPawn->Move(FVector::BackwardVector, Scale);
		*/

	if (FMath::IsNearlyEqual(MousePos.X, 0, 10.0f) && !DisableCameraMovements)
		PlayerPawn->Move(false, Scale * -1);

	else if (FMath::IsNearlyEqual(MousePos.X, ScreenSize.X, 10.0f) && !DisableCameraMovements)
		PlayerPawn->Move(false, Scale);

	if (FMath::IsNearlyEqual(MousePos.Y, 0, 10.0f) && !DisableCameraMovements)
		PlayerPawn->Move(true, Scale);

	else if (FMath::IsNearlyEqual(MousePos.Y, ScreenSize.Y, 10.0f) && !DisableCameraMovements)
		PlayerPawn->Move(true, Scale * -1);

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
	PlayerPawn->Zoom(Axis);

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

	else if (SpringArmRotation.Pitch > MaxPitchAngle)
		PlayerPawn->SetSpringArmPitchRotation(MaxPitchAngle, MinPitchAngle, MaxPitchAngle);
}

void AMainPlayerController::MoveKeyboardForward(float Axis)
{
	if (DisablePawnControl)
		return;
	/*
	if (InputComponent->GetAxisValue("KeyboardMoveForward") == 1.0f)
		PlayerPawn->Move(FVector::ForwardVector, ZoomFactor);
	else if (InputComponent->GetAxisValue("KeyboardMoveForward") == -1.0f)
		PlayerPawn->Move(FVector::BackwardVector, ZoomFactor);
	else
		return;
*/

	if (InputComponent->GetAxisValue("KeyboardMoveForward") == 1.0f)
		PlayerPawn->Move(true, SCALE_PAWN_MOVEMENT);
	else if (InputComponent->GetAxisValue("KeyboardMoveForward") == -1.0f)
		PlayerPawn->Move(true, SCALE_PAWN_MOVEMENT * -1);
	else
		return;
}

void AMainPlayerController::MoveKeyboardRight(float Axis)
{
	if (DisablePawnControl)
		return;
	/*
	if (InputComponent->GetAxisValue("KeyboardMoveRight") == 1.0f)
		PlayerPawn->Move(FVector::RightVector, ZoomFactor);
	else if (InputComponent->GetAxisValue("KeyboardMoveRight") == -1.0f)
		PlayerPawn->Move(FVector::LeftVector, ZoomFactor);
	else
		return;
		*/

	if (InputComponent->GetAxisValue("KeyboardMoveRight") == 1.0f)
		PlayerPawn->Move(false, SCALE_PAWN_MOVEMENT);
	else if (InputComponent->GetAxisValue("KeyboardMoveRight") == -1.0f)
		PlayerPawn->Move(false, SCALE_PAWN_MOVEMENT * -1);
	else
		return;
}

void AMainPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Updating position of the mouse at each frame
	MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(World) * UWidgetLayoutLibrary::GetViewportScale(World);

	// PrintString(FString::Printf(TEXT("Mouse : %s"), *MousePos.ToString()));

	// Updating screen size at each frame
	GEngine->GameViewport->GetViewportSize(ScreenSize);
	// Checking for EdgeScrolling at each frame
	MouseEdgeScrolling();
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

UUI_ConstructionMain* AMainPlayerController::GetConstructionWidget()
{

#ifdef DEBUG_ONLY

	checkf(ConstructionWidget, TEXT("Error in AMainPlayerController::GetConstructionWidget, ConstructionWidget == nullptr "));

#endif

	return ConstructionWidget;
}

UUI_Main* AMainPlayerController::GetMainWidget()
{

#ifdef DEBUG_ONLY

	checkf(MainWidget, TEXT("Error in AMainPlayerController::GetMainWidget, MainWidget == nullptr "));

#endif

	return MainWidget;
}

UUI_BuildingInfos* AMainPlayerController::GetBuildingInfos() const
{
	return BuildingInfosWidget;
}

void AMainPlayerController::SetMainWidget(UUI_Main* Widget)
{

#ifdef DEBUG_ONLY

	checkf(Widget, TEXT("Error in AMainPlayerController::SetMainWidget, Widget == nullptr "));

#endif

	MainWidget = Widget;
}

void AMainPlayerController::SetBuildingInfosWidget(UUI_BuildingInfos* Widget)
{

#ifdef DEBUG_ONLY

	checkf(Widget, 
		TEXT("Error in AMainPlayerController::SetBuildingInfosWidget, Widget == nultpr. Check that this ptr is set in BuildingInfos Construct."));

#endif

	BuildingInfosWidget = Widget;
}

void AMainPlayerController::SetGraphWidget(UUI_GraphsMain* Widget)
{

#ifdef DEBUG_ONLY

	checkf(Widget, TEXT("Error in AMainPlayerController::SetGraphWidget, Widget == nullptr "));

#endif

	GraphsWidget = Widget;
}

void AMainPlayerController::SetConstructionWidget(UUI_ConstructionMain* Widget)
{
#ifdef DEBUG_ONLY

	checkf(Widget, TEXT("Error in AMainPlayerController::SetConstructionWidget, Widget == nullptr "));

#endif

	ConstructionWidget = Widget;
}


