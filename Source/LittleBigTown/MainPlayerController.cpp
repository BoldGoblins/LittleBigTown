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


AMainPlayerController::AMainPlayerController()
{
}

void AMainPlayerController::BeginPlay()
{

	Super::BeginPlay();

	ZoomScale = ZOOM_MIN;
	PlayerPawn = Cast <APlayerPawn> (GetPawn());
	bEnableClickEvents = true;
	SetupInputComponent();
	// SetShowMouseCursor(true);

}

void AMainPlayerController::MouseEdgeScrolling (const FVector2D & Mouse, const FVector2D & Screen, float LimitX, float LimitY)
{
	const auto CheckAxis{ [&] (float MouseAxis, float ScreenSizeAxis, float LimitScreenAxis) -> void
		{
			if ((MouseAxis < (0 + LimitScreenAxis)) && !DisableCameraMovements)
			{
				float ScaleValue { FMath::Abs((FMath::Clamp(MouseAxis / LimitScreenAxis, 0.0f, 1.0f) - 1)) };
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%f"), ScaleValue * ZoomFactor));
				PlayerPawn->Move((ScreenSizeAxis == Screen.X ? FVector::LeftVector : FVector::ForwardVector), ScaleValue * ZoomFactor);
			}

			if ((MouseAxis > (ScreenSizeAxis - LimitScreenAxis)) && !DisableCameraMovements)
			{
				float ScaleValue { FMath::Abs((FMath::Clamp((ScreenSizeAxis - MouseAxis) / LimitScreenAxis, 0.0f, 1.0f) - 1)) };
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%f"), ScaleValue * ZoomFactor));
				PlayerPawn->Move((ScreenSizeAxis == Screen.X ? FVector::RightVector : FVector::BackwardVector), ScaleValue * ZoomFactor);
			}
		} };

	CheckAxis(Mouse.X, Screen.X, LimitX);
	CheckAxis(Mouse.Y, Screen.Y, LimitY);
}


void AMainPlayerController::AddZoom()
{
	ZoomScale = FMath::Clamp(--ZoomScale, ZOOM_MAX, ZOOM_MIN);
	ZoomFactor = FMath::Clamp(ZoomFactor -= ZOOM_FACTOR_VARIATION, ZOOM_FACTOR_MIN, ZOOM_FACTOR_MAX);
	PlayerPawn->Zoom(ZoomScale);
}

void AMainPlayerController::DecreaseZoom()
{
	ZoomScale = FMath::Clamp(++ZoomScale, ZOOM_MAX, ZOOM_MIN);
	ZoomFactor = FMath::Clamp(ZoomFactor += ZOOM_FACTOR_VARIATION, ZOOM_FACTOR_MIN, ZOOM_FACTOR_MAX);
	PlayerPawn->Zoom(ZoomScale);
}



void AMainPlayerController::Tick(float DeltaTime)
{
	MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(Super::GetWorld());
	MouseEdgeScrolling (MousePos, ScreenSize, borderX, borderY);
}


void AMainPlayerController::SetupInputComponent()
{

	Super::SetupInputComponent();
	
	if (PlayerPawn)
	{
		InputComponent->BindAction(FName("ZoomIn"), IE_Pressed, this, &AMainPlayerController::AddZoom);
		InputComponent->BindAction(FName("ZoomOut"), IE_Pressed, this, &AMainPlayerController::DecreaseZoom);
	}
	
	
}

