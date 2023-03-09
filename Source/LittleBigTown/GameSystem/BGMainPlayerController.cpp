// Fill out your copyright notice in the Description page of Project Settings.


#include "LittleBigTown/GameSystem/BGMainPlayerController.h"

#include "Blueprint/WidgetLayoutLibrary.h"

#include "LittleBigTown/UserInterface/Main/UI_Main.h"
#include "LittleBigTown/UserInterface/BuildingInfos/UI_BuildingInfos.h"
#include "LittleBigTown/UserInterface/Graphs/UI_GraphsMain.h"
#include "LittleBigTown/UserInterface/Construction/UI_ConstructionMain.h"
#include "LittleBigTown/Actors/BGPlayerPawn.h"
#include "LittleBigTown/GameSystem/MainGameState.h"
#include "LittleBigTown/GameSystem/MainGameMode.h"
#include "LittleBigTown/Core/Debugger.h"

#define VP_MIDDLE_X (_ViewportSize.X / 2.0f)
#define VP_MIDDLE_Y (_ViewportSize.Y / 2.0f)
#define VP_EDGE_BORDER_X (_ViewportSize.X * 10) / 100.0f
#define VP_EDGE_BORDER_Y (_ViewportSize.Y * 10) / 100.0f
#define FPS_CORRECTION (GetWorld()->DeltaTimeSeconds * 60)

ABGMainPlayerController::ABGMainPlayerController()
{
	InputModeGameAndUI.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
}

void ABGMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	GEngine->GameViewport->Viewport->ViewportResizedEvent.AddUObject(this, &ThisClass::SetViewportSize);

	_PlayerPawn = Cast <ABGPlayerPawn> (GetPawn());

	// Réinitialiser Variables qui sont dépendent d'autres qui Set depuis l'Editeur
	_CurrentZoomNotch = _MaxZoomNotches;

	SetInputMode(InputModeGameAndUI);


#ifdef DEBUG_ONLY

	checkf(_PlayerPawn.IsValid(), TEXT("Error in ABGMainPlayerController::BeginPlay, PlayerPawn not valid."));

#endif
}

void ABGMainPlayerController::Tick(float DeltaTime)
{
	if (!bEdgeScrollDisabled)
		EdgeScrolling();
}

void ABGMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// AXIS : 
	InputComponent->BindAxis(FName("KeyboardMoveForward"), this, &ThisClass::ScrollForward);
	InputComponent->BindAxis(FName("KeyboardMoveRight"), this, &ThisClass::ScrollRight);
	InputComponent->BindAxis(FName("Zoom"), this, &ThisClass::Zoom);
	InputComponent->BindAxis(FName("YawRotation"), this, &ThisClass::YawRotation);
	InputComponent->BindAxis(FName("PitchRotation"), this, &ThisClass::PitchRotation);
	InputComponent->BindAxis(FName("KeyboardYawRotation"), this, &ThisClass::KeyboardYawRotation);
	InputComponent->BindAxis(FName("KeyboardPitchRotation"), this, &ThisClass::KeyboardPitchRotation);

	// ACTIONS : 
	InputComponent->BindAction(FName("CameraRotation"), IE_Pressed, this, &ThisClass::EnableMouseCaptureRotation);
	InputComponent->BindAction(FName("CameraRotation"), IE_Released, this, &ThisClass::DisableMouseCaptureRotation);

}

void ABGMainPlayerController::SetViewportSize(FViewport* ViewPort, uint32 Val)
{
	// PrintString(FString::Printf(TEXT("Viewport : %s"), *ViewPort->GetSizeXY().ToString()));
	_ViewportSize = ViewPort->GetSizeXY();
}

void ABGMainPlayerController::ScrollForward(float Axis)
{
	if (_PlayerPawn.IsValid() && !FMath::IsNearlyZero(Axis, 0.001f))
		_PlayerPawn->Move(Axis * _EdgeScrollingThreshold * _ScrollCorrection * FPS_CORRECTION, 0.0f);
}

void ABGMainPlayerController::ScrollRight(float Axis)
{
	if (_PlayerPawn.IsValid() && !FMath::IsNearlyZero(Axis, 0.001f))
		_PlayerPawn->Move(0.0f, Axis * _EdgeScrollingThreshold * _ScrollCorrection * FPS_CORRECTION);
}

void ABGMainPlayerController::PitchRotation(float Axis)
{
	if (bEdgeScrollDisabled && !bShowMouseCursor)
	{
		// PrintString(FString::Printf(TEXT("Pitch : %f"), Axis));
		if (_PlayerPawn.IsValid())
			_PlayerPawn->AddPitchRotation(Axis, _CurrentZoomNotch, _MaxZoomNotches);
	}

}

void ABGMainPlayerController::YawRotation(float Axis)
{
	if (bEdgeScrollDisabled && !bShowMouseCursor)
	{
		// PrintString(FString::Printf(TEXT("Yaw : %f"), Axis));
		if (_PlayerPawn.IsValid())
			_PlayerPawn->AddYawRotation(Axis);
	}
}

void ABGMainPlayerController::KeyboardPitchRotation(float Axis)
{
	if (FMath::IsNearlyZero(Axis))
		return;

	if (_PlayerPawn.IsValid())
		_PlayerPawn->AddPitchRotation(Axis, _CurrentZoomNotch, _MaxZoomNotches);
}

void ABGMainPlayerController::KeyboardYawRotation(float Axis)
{
	if (FMath::IsNearlyZero(Axis))
		return;

	if (_PlayerPawn.IsValid())
		_PlayerPawn->AddYawRotation(Axis);
}

void ABGMainPlayerController::EnableMouseCaptureRotation()
{
	bShowMouseCursor = false;
	bEdgeScrollDisabled = true;
	SetInputMode(InputModeGameOnly);
}

void ABGMainPlayerController::DisableMouseCaptureRotation()
{
	SetInputMode(InputModeGameAndUI);
	bShowMouseCursor = true;
	bEdgeScrollDisabled = false;
}

void ABGMainPlayerController::Zoom(float Axis)
{
	if ((_CurrentZoomNotch == 0 && Axis < 0) || (_CurrentZoomNotch == _MaxZoomNotches && Axis > 0))
		return;

	_CurrentZoomNotch = FMath::Clamp(FMath::RoundToInt(_CurrentZoomNotch + Axis), 0, _MaxZoomNotches);
	_ScrollCorrection = _MinScrollCorrection + (_CurrentZoomNotch * ((1 - _MinScrollCorrection) / _MaxZoomNotches));

	// PrintString(FString::Printf(TEXT("Correction = %f, Notch %d"), _ScrollCorrection, _CurrentZoomNotch));

	if(_PlayerPawn.IsValid())
	{
		bool bAutoPitch { _PlayerPawn->ShouldUseAutoPitch(false) };

		// _PlayerPawn->ComputeSpringArmLength(_CurrentZoomNotch, _MaxZoomNotches);
		_PlayerPawn->SetCameraDistance(_CurrentZoomNotch, _MaxZoomNotches);
		_PlayerPawn->UpdateMaxPitchAngle(_CurrentZoomNotch, _MaxZoomNotches);

		// Deux Checks : un si le Pitch est déjà == minimum, l'autre si le Pitch est > minimum lors du dézoom
		if (bAutoPitch || _PlayerPawn->ShouldUseAutoPitch(true))
			_PlayerPawn->AutoPitchRotation();

	}
}

void ABGMainPlayerController::EdgeScrolling()
{
	// APlayerController::GetMousePosition() donne des résultats étranges en Standalone
	// Penser à bien multiplier par ViewportScale
	FVector2D MousePos { UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld()) * UWidgetLayoutLibrary::GetViewportScale(GetWorld()) };

	float ForwardAxis { 0.0f };
	float RightAxis { 0.0f };

	if (MousePos.X < VP_EDGE_BORDER_X || MousePos.X >(_ViewportSize.X - VP_EDGE_BORDER_X))
	{
		RightAxis = MousePos.X < VP_EDGE_BORDER_X ? MousePos.X - VP_EDGE_BORDER_X : MousePos.X - (_ViewportSize.X - VP_EDGE_BORDER_X);
		RightAxis /= VP_EDGE_BORDER_X;

		ForwardAxis = (VP_MIDDLE_Y - MousePos.Y) / VP_MIDDLE_Y;
	}

	else if (MousePos.Y < VP_EDGE_BORDER_Y || FMath::IsNearlyEqual(MousePos.Y, _ViewportSize.Y, 10.0f))
	{
		ForwardAxis = MousePos.Y < VP_EDGE_BORDER_Y ? VP_EDGE_BORDER_Y - MousePos.Y : -VP_EDGE_BORDER_Y;
		ForwardAxis /= VP_EDGE_BORDER_Y;

		RightAxis = (MousePos.X - VP_MIDDLE_X) / VP_MIDDLE_X;
	}
	else
		return;

	// PrintString(FString::Printf(TEXT("Déplacement X : %f"), ForwardAxis * _EdgeScrollingThresold * FPS_CORRECTION));
	// PrintString(FString::Printf(TEXT("Déplacement Y : %f"), RightAxis * _EdgeScrollingThresold * FPS_CORRECTION));

	if (_PlayerPawn.IsValid())
		_PlayerPawn->Move(ForwardAxis * _EdgeScrollingThreshold * _ScrollCorrection * FPS_CORRECTION,
			RightAxis * _EdgeScrollingThreshold * _ScrollCorrection * FPS_CORRECTION);
}

UUI_ConstructionMain* ABGMainPlayerController::GetConstructionWidget()
{

#ifdef DEBUG_ONLY

	checkf(_ConstructionWidget, TEXT("Error in ABGMainPlayerController::GetConstructionWidget, ConstructionWidget == nullptr "));

#endif

	return _ConstructionWidget;
}

UUI_Main* ABGMainPlayerController::GetMainWidget()
{

#ifdef DEBUG_ONLY

	checkf(_MainWidget, TEXT("Error in ABGMainPlayerController::GetMainWidget, MainWidget == nullptr "));

#endif

	return _MainWidget;
}

UUI_GraphsMain* ABGMainPlayerController::GetGraphsWidget() const
{

#ifdef DEBUG_ONLY

	checkf(_GraphsWidget, TEXT("Error in ABGMainPlayerController::GetMainWidget, GraphsWidget == nullptr "));

#endif

	return _GraphsWidget;
}

UUI_BuildingInfos* ABGMainPlayerController::GetBuildingInfos() const
{

#ifdef DEBUG_ONLY

	checkf(_BuildingInfosWidget, TEXT("Error in ABGMainPlayerController::GetBuildingInfos, BuildingInfosWidget == nullptr "));

#endif

	return _BuildingInfosWidget;
}
void ABGMainPlayerController::SetMainWidget(UUI_Main* Widget)
{

#ifdef DEBUG_ONLY

	checkf(IsValid(Widget), TEXT("Error in ABGMainPlayerController::SetMainWidget, tentative de passer un Widget non-valide."));

#endif

	_MainWidget = Widget;
}

void ABGMainPlayerController::SetBuildingInfosWidget(UUI_BuildingInfos* Widget)
{

#ifdef DEBUG_ONLY

	checkf(IsValid(Widget), TEXT("Error in ABGMainPlayerController::SetBuildingInfosWidget, tentative de passer un Widget non-valide."));

#endif

	_BuildingInfosWidget = Widget;
}

void ABGMainPlayerController::SetGraphWidget(UUI_GraphsMain* Widget)
{
#ifdef DEBUG_ONLY

	checkf(IsValid(Widget), TEXT("Error in ABGMainPlayerController::SetGraphWidget, tentative de passer un Widget non-valide."));

#endif

	_GraphsWidget = Widget;
}

void ABGMainPlayerController::SetConstructionWidget(UUI_ConstructionMain* Widget)
{
#ifdef DEBUG_ONLY

	checkf(IsValid(Widget), TEXT("Error in ABGMainPlayerController::SetConstructionWidget, tentative de passer un Widget non-valide."));

#endif

	_ConstructionWidget = Widget;
}
