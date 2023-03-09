// Fill out your copyright notice in the Description page of Project Settings.


#include "LittleBigTown/Actors/BGPlayerPawn.h"
// Fill out your copyright notice in the Description page of Project Settings.

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SphereComponent.h"
// #include "Kismet/GameplayStatics.h"
#include "Components/ArrowComponent.h"
#include "DrawDebugHelpers.h"
#include "LittleBigTown/GameSystem/BGMainPlayerController.h"
#include "LittleBigTown/Actors/Building.h"

#include "LittleBigTown/Core/Debugger.h"

// Ajouté en permanence à la longueur du SA, permet d'éviter de se retrouver avec une longueur de SA == 0 (induit problème Pitch Rotation).
#define SPRING_ARM_LENGTH_MARGIN 50.0f
#define DEFAULT_PITCH_VAR_ANGLE 30.0f
#define MIN_DISTANCE_UNDER_CAMERA 50
#define Z_AXIS_MOVEMENT 100
// #define DISTANCE_CENTER_TO_CAMERA FMath::Sqrt(FMath::Square(_SphereComp->GetComponentLocation().X - _CameraComp->GetComponentLocation().X) + FMath::Square(_SphereComp->GetComponentLocation().Y + _CameraComp->GetComponentLocation().Y) + FMath::Square(_SphereComp->GetComponentLocation().Z - _CameraComp->GetComponentLocation().Z))
#define DISTANCE_CENTER_TO_CAMERA FVector::Distance(_SphereComp->GetComponentLocation(), _CameraCollisionSphere->GetComponentLocation())
#define TRANSFORM_CAMERA (_PitchArrowComp->GetForwardVector() * -1) * _TargetSpringArmLength
#define FLOAT_TOLERANCE_ERROR 0.00f

// Sets default values
ABGPlayerPawn::ABGPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	// Construction hidden Static Mesh
	_SphereComp = CreateDefaultSubobject <USphereComponent>(TEXT("Hidden Sphere"));
	SetRootComponent(_SphereComp);

	// Construction des 2 Arrows Components
	_YawArrowComp = CreateDefaultSubobject <UArrowComponent>(TEXT("Arrow Component"));
	_YawArrowComp->SetupAttachment(_SphereComp);

	_PitchArrowComp = CreateDefaultSubobject <UArrowComponent>(TEXT("Pitch Arrow"));
	_PitchArrowComp->SetupAttachment(_YawArrowComp);
	_PitchArrowComp->SetRelativeRotation(FRotator(-85.0f, 0.0f, 0.0f));

	// Construction Sphere Collision
	_CameraCollisionSphere = CreateDefaultSubobject <USphereComponent>(TEXT("Camera Collision Sphere"));
	_CameraCollisionSphere->SetupAttachment(_PitchArrowComp);
	_CameraCollisionSphere->SetSphereRadius(128);

	// Construction Camera
	_CameraComp = CreateDefaultSubobject <UCameraComponent>(TEXT("Camera"));
	_CameraComp->SetupAttachment(_CameraCollisionSphere);
}

// Called when the game starts or when spawned
void ABGPlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	_TargetSpringArmLength = _DefaultSpringArmLength + SPRING_ARM_LENGTH_MARGIN;
	_CameraCollisionSphere->SetWorldLocation(_SphereComp->GetComponentLocation() + TRANSFORM_CAMERA);
	_MinPitchAngle = _PitchArrowComp->GetComponentRotation().Pitch;
	_TargetYawRotation = _YawArrowComp->GetComponentRotation().Yaw;

	_CameraCollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBegin);
	_CameraCollisionSphere->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnOverlapEnd);

	const auto PlayerController{ Cast <ABGMainPlayerController>(Controller) };

	if (IsValid(PlayerController))
		UpdateMaxPitchAngle(PlayerController->GetZoomNotchesNum(), PlayerController->GetZoomNotchesNum());

	AutoPitchRotation();

#ifdef DEBUG_ONLY

	checkf(_MaxPitchAngle <= 0.0f && _MinPitchAngle <= 0.0f, TEXT("Error in ABGPlayerPawn::BeginPlay, SpringArmComp Rotation ne peut être > 0."));
	checkf(_MaxPitchAngle <= 0.0f, TEXT("Error in ABGPlayerPawn::BeginPlay, _MinPitchAngle ne peut être > 0."));
	checkf(FMath::Abs(_MinPitchAngle) - FMath::Abs(_MaxPitchAngle) >= DEFAULT_PITCH_VAR_ANGLE,
		TEXT("Error in ABGPlayerPawn::BeginPlay, écart entre Min et Max Pitch Angle doit être > DEFAULT_PITCH_VAR_ANGLE."));
	checkf(IsValid(PlayerController), TEXT("Error in ABGPlayerPawn::BeginPlay, PlayerController invalid."));

#endif

}

// Called every frame
void ABGPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// PrintString(FString::Printf(TEXT("Location : %s, Target : %s"), *GetActorLocation().ToString(), *_TargetLocation.ToString()));

	// ACTOR LOCATION INTERP : 
	if (!GetActorLocation().Equals(_TargetLocation, FLOAT_TOLERANCE_ERROR))
		SetActorLocation(FMath::VInterpTo(GetActorLocation(), _TargetLocation, DeltaTime, _InterpMoveSpeed), true);

	// ZOOM INTERP
	if (!FMath::IsNearlyEqual(_TargetSpringArmLength, DISTANCE_CENTER_TO_CAMERA, FLOAT_TOLERANCE_ERROR))
		_CameraCollisionSphere->SetWorldLocation(FMath::VInterpTo(_CameraCollisionSphere->GetComponentLocation(),
			_SphereComp->GetComponentLocation() + TRANSFORM_CAMERA, DeltaTime, _InterpZoomSpeed), true);

	// YAW ROTATION INTERP
	if (!FMath::IsNearlyEqual(_TargetYawRotation, _YawArrowComp->GetRelativeRotation().Yaw, FLOAT_TOLERANCE_ERROR))
		_YawArrowComp->SetRelativeRotation(FRotator(0.0f, FMath::FInterpTo(_YawArrowComp->GetRelativeRotation().Yaw, _TargetYawRotation, 
			DeltaTime, _InterpRotationSpeed), 0.0f), true);

	// PITCH ROTATION INTERP
	if (!FMath::IsNearlyEqual(_TargetPitchRotation, _PitchArrowComp->GetRelativeRotation().Pitch, FLOAT_TOLERANCE_ERROR))
		_PitchArrowComp->SetRelativeRotation(FRotator(FMath::FInterpTo(_PitchArrowComp->GetRelativeRotation().Pitch, _TargetPitchRotation,
			DeltaTime, _InterpRotationSpeed), 0.0f, 0.0f), true);

	// FREE CAMERA Z LOCATION INTERP
	if (!FMath::IsNearlyEqual(_TargetFreeCameraLocZ, _CameraComp->GetRelativeLocation().Z, FLOAT_TOLERANCE_ERROR))
		_CameraComp->SetRelativeLocation(FVector(0.0f, 0.0f, FMath::FInterpTo(_CameraComp->GetRelativeLocation().Z,
			_TargetFreeCameraLocZ, DeltaTime, _InterpMoveSpeed)));

	// FREE CAMERA PITCH ROTATION INTERP
	if (!FMath::IsNearlyEqual(_TargetFreeCameraPitch, _CameraComp->GetRelativeRotation().Pitch, FLOAT_TOLERANCE_ERROR))
		_CameraComp->SetRelativeRotation(FRotator(FMath::FInterpTo(_CameraComp->GetRelativeRotation().Pitch, _TargetFreeCameraPitch, DeltaTime,
			_InterpRotationSpeed), 0.0f, 0.0f));
}

// Called to bind functionality to input
void ABGPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
/*
*
* OLD FUNCTION MOVE
*
void ABGPlayerPawn::Move(float ForwardAxis, float RightAxis)
{
	FVector Modifier { (_YawArrowComp->GetForwardVector() * ForwardAxis) + (_YawArrowComp->GetRightVector() * RightAxis) };
	SetActorLocation(GetActorLocation() + Modifier, true);
}

*/
void ABGPlayerPawn::Move(float ForwardAxis, float RightAxis)
{
	FVector Modifier{ (_YawArrowComp->GetForwardVector() * ForwardAxis) + (_YawArrowComp->GetRightVector() * RightAxis) };

	_TargetLocation = (GetActorLocation() + Modifier);
}
/*
* OLD YAW ROTATION
*
void ABGPlayerPawn::AddYawRotation(float Axis)
{
	FRotator R { _ArrowComp->GetRelativeRotation() };

	R.Add(0.0f, Axis, 0.0f);
	_ArrowComp->SetRelativeRotation(R);

	// AutoAltitude();
}
*/

void ABGPlayerPawn::AddYawRotation(float Axis)
{
	FRotator R { _YawArrowComp->GetRelativeRotation() };

	_TargetYawRotation = R.Yaw + Axis;
}

/*
*
* OLD PITCH ROTATION FUNCTION
*
void ABGPlayerPawn::AddPitchRotation(float Axis, int32 ZoomNotch, int32 MaxZoomNotches)
{
	FRotator R { _SpringArmComp->GetRelativeRotation() };

	R.Pitch = FMath::Clamp(R.Pitch + Axis, _MaxPitchAngle, _CurrentMinPitchAngle);

	// PrintString(FString::Printf(TEXT("%f, Min : %f, Max : %f"), R.Pitch, _CurrentMinPitchAngle, _MaxPitchAngle));

	_SpringArmComp->SetRelativeRotation(R);

	// AutoAltitude();
}
*/

void ABGPlayerPawn::AddPitchRotation(float Axis, int32 ZoomNotch, int32 MaxZoomNotches)
{
	FRotator R { _PitchArrowComp->GetRelativeRotation() };

	if (!_bIsCameraFreed)
	{
		// R.Pitch = FMath::Clamp(R.Pitch + Axis, _MinPitchAngle, _CurrentMaxPitchAngle);
		_TargetPitchRotation = FMath::Clamp(R.Pitch + Axis, _MinPitchAngle, _CurrentMaxPitchAngle);
	}
	else
		_TargetFreeCameraPitch = FMath::Clamp(Axis + _CameraComp->GetRelativeRotation().Pitch, _MinPitchAngle - R.Pitch,
			_CurrentMaxPitchAngle - R.Pitch);
}
/*
*
* OLD AUTO PITCH
*
void ABGPlayerPawn::AutoPitchRotation()
{
	FRotator R { _SpringArmComp->GetRelativeRotation() };
	R.Pitch = _CurrentMinPitchAngle;

	_SpringArmComp->SetRelativeRotation(R);
}
*/

void ABGPlayerPawn::AutoPitchRotation()
{
	_TargetPitchRotation = _CurrentMaxPitchAngle;
}

/*
*
* OLD AUTO PITCH
*
bool ABGPlayerPawn::ShouldUseAutoPitch(bool bAfterZooming)
{
	if (!bAfterZooming)
		return (FMath::IsNearlyEqual(_SpringArmComp->GetComponentRotation().Pitch, _CurrentMinPitchAngle, 5.0f));

	else // Si on est plus bas que ce que l'on devrait
		return _SpringArmComp->GetComponentRotation().Pitch > _CurrentMinPitchAngle;
}
*/

bool ABGPlayerPawn::ShouldUseAutoPitch(bool bAfterZooming)
{
	if (!bAfterZooming)
		return (FMath::IsNearlyEqual(_PitchArrowComp->GetComponentRotation().Pitch, _CurrentMaxPitchAngle, 5.0f));

	else // Si on est plus bas que ce que l'on devrait
		return _PitchArrowComp->GetComponentRotation().Pitch > _CurrentMaxPitchAngle;
}

void ABGPlayerPawn::UpdateMaxPitchAngle(int32 ZoomNotch, int32 MaxZoomNotches)
{
	_CurrentMaxPitchAngle = _MaxPitchAngle + ((ZoomNotch * ((_MinPitchAngle + DEFAULT_PITCH_VAR_ANGLE) - _MaxPitchAngle)) / MaxZoomNotches);
}

void ABGPlayerPawn::SetCameraDistance(int32 ZoomNotch, int32 MaxZoomNotches)
{
	_TargetSpringArmLength = ZoomNotch * (_DefaultSpringArmLength / MaxZoomNotches) + SPRING_ARM_LENGTH_MARGIN;
}

void ABGPlayerPawn::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor))
		return;

	if (OtherActor->IsA <ABuilding>())
	{
		// PrintString(TEXT("Deplacer Camera !!!"));

		const auto Building{ Cast <ABuilding>(OtherActor) };

		if (IsValid(Building))
		{
			_TargetFreeCameraLocZ = Building->GetBuildingHeight() - _CameraComp->GetComponentLocation().Z;
			_bIsCameraFreed = true;
		}
	}
}

void ABGPlayerPawn::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// PrintString(TEXT("Remettre Camera en place !!!"));

	_TargetFreeCameraLocZ = 0.0f;
	_TargetFreeCameraPitch = 0.0f;
	_bIsCameraFreed = false;
}
