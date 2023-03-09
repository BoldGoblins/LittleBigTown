// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "BGPlayerPawn.generated.h"

UCLASS()
class LITTLEBIGTOWN_API ABGPlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABGPlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(float ForwardAxis, float RightAxis);

	void AddYawRotation(float Axis);

	void AddPitchRotation(float Axis, int32 ZoomNotch, int32 MaxZoomNotches);

	void AutoPitchRotation();

	// bAfterZooming : Si on utilise cette fonction avant (false) ou après (true) avoir notifié le Pawn du Zoom (et avoir changé son MinPitchAngle)
	bool ShouldUseAutoPitch(bool bAfterZooming);

	void UpdateMaxPitchAngle(int32 ZoomNotch, int32 MaxZoomNotches);

	// Calcule distance de la Camera au Pawn en fonction du Zoom (simule SpringArm)
	void SetCameraDistance(int32 ZoomNotch, int32 MaxZoomNotches);

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);



protected:


	// --------------------------------------		PAWN COMPONENTS		  --------------------------------------


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class UCameraComponent* _CameraComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class USphereComponent* _SphereComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class USphereComponent* _CameraCollisionSphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class UArrowComponent* _YawArrowComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class UArrowComponent* _PitchArrowComp;	

	// Angle Maximal de Rotation Pitch de la Camera (Angle Minimal Set directement depuis l'Editeur sur le PitchArrowComp)
	UPROPERTY(EditDefaultsOnly)
		float _MaxPitchAngle { - 15.0f };

	// Distance Maximale entre la Camera et l'Origin (le RootComponent = la Sphere) du Pawn
	UPROPERTY(EditDefaultsOnly)
		float _DefaultSpringArmLength { 10000.0f };

	// Interpolation de la Rotation, plus la valeur est élevée, plus les changements d'angle de la Caméra s'effectuent de manière brusque
	UPROPERTY(EditDefaultsOnly) 
		float _InterpRotationSpeed { 35.0f };

	// Interpolation de la Rotation, plus la valeur est élevée, plus le Zoom produit un effet "haché"
	UPROPERTY(EditDefaultsOnly)
		float _InterpZoomSpeed{ 10.0f };

	// Interpolation de la Rotation, plus la valeur est élevée, plus les déplacements s'effectuent de manière brusque
	UPROPERTY(EditDefaultsOnly)
		float _InterpMoveSpeed{ 10.0f };

	float _CurrentMaxPitchAngle {};

	float _MinPitchAngle{};

	float _TargetSpringArmLength {};

	float _TargetFreeCameraLocZ { 0.0f };

	float _TargetFreeCameraPitch { 0.0f };

	bool _bIsCameraFreed { false };

	float _TargetPitchRotation {};

	float _TargetYawRotation {};

	FVector _TargetLocation{};
	 
};
