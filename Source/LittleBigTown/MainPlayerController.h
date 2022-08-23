// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
// For definitions
#include "LittleBigTown.h"

#include "MainPlayerController.generated.h"

/**
 * 
 */

UCLASS()
class LITTLEBIGTOWN_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public : 

		AMainPlayerController();
		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;
		virtual void SetupInputComponent() override;

		// Normalement pas besoin de check les FPS, à vérifier /!\

		void MouseEdgeScrolling (const FVector2D& Pos, const FVector2D& Size, float LimitX, float LimitY);

		void AddZoom();
		void DecreaseZoom();


protected :
	
	UPROPERTY(BlueprintReadOnly)
		class APlayerPawn* PlayerPawn {};

	UPROPERTY(BlueprintReadOnly)
		FVector2D MousePos {};

	UPROPERTY(BlueprintReadOnly)
		float ZoomFactor { 1.000f };

	UPROPERTY(BlueprintReadWrite, Transient)
		FVector2D ScreenSize;

	// Border (10% of ScreenSize.X) where to trigger Pawn movement. Edit via BP method.
	UPROPERTY(BlueprintReadWrite, Transient)
		float borderX;

	// Border (10% of ScreenSize.Y) where to trigger Pawn movement. Edit via BP method.
	UPROPERTY(BlueprintReadWrite, Transient)
		float borderY;

	// Compute the length of SpringArm
	UPROPERTY(BlueprintReadOnly, Transient)
		int ZoomScale;

	UPROPERTY(BlueprintReadWrite)
		bool DisableCameraMovements;

	
};
