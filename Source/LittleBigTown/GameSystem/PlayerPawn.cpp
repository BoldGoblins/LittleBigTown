// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPawn.h"
// UCameraComponent
#include "Camera/CameraComponent.h"
// USpringArmComponent
#include "GameFramework/SpringArmComponent.h"
// RootComponent
#include "Components/SceneComponent.h"
// PawnMovement
#include "GameFramework/FloatingPawnMovement.h"
// Sphere
#include "Components/SphereComponent.h"
// UGameplayStatics::GetPlayerController()
#include "Kismet/GameplayStatics.h"
// Arrow comp
#include "Components/ArrowComponent.h"
// DrawDebug
#include "DrawDebugHelpers.h"
#include "LittleBigTown/Core/Debugger.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Get Player Controller Ref
	PlayerController = Cast <AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	// Construction hidden Static Mesh
	SphereComp = CreateDefaultSubobject <USphereComponent>(TEXT("Hidden Sphere"));
	SetRootComponent(SphereComp);

	// Construction ArrowComponent
	ArrowComp = CreateDefaultSubobject <UArrowComponent>(TEXT("Arrow Component"));
	ArrowComp->AttachToComponent(SphereComp, FAttachmentTransformRules::KeepRelativeTransform);

	// Construction SpringArm
	SpringArmComp = CreateDefaultSubobject <USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->AttachToComponent(ArrowComp, FAttachmentTransformRules::KeepRelativeTransform);
	SpringArmComp->TargetArmLength = DEFAULT_SPRING_ARM_LENGTH;

	// Construction Camera
	CameraComp = CreateDefaultSubobject <UCameraComponent>(TEXT("Camera"));
	CameraComp->AttachToComponent(SpringArmComp, FAttachmentTransformRules::KeepRelativeTransform);

	// Construction PawnMovement
	PawnMovement = CreateDefaultSubobject <UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
	PawnMovement->MaxSpeed = MAX_SPEED;
	PawnMovement->Acceleration = ACCELERATION;
	PawnMovement->Deceleration = DECELERATION;


}
// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	// Set default rotation for Spring Arm 
	SpringArmComp->SetRelativeRotation(FRotator(DEFAULT_PITCH_ROTATION_PAWN, 0.0f, 0.0f));

}

bool APlayerPawn::CollisionQueryAlongXYAxis()
{
	const FVector PawnLoc{ this->GetActorLocation() };
	const FVector Forward{ ArrowComp->GetForwardVector() };

	for (int i{ 0 }; i < 8; i++)
	{
		FHitResult Result{};
		FCollisionQueryParams CollisionParams;

		float Angle { static_cast <float> (360 / 8) * i };
		// Optimiser (end = start + 250)
		FVector Start{ Forward.RotateAngleAxis(Angle, ArrowComp->GetUpVector()) };
		Start *= 50;
		Start += PawnLoc;

		FVector End{ Forward.RotateAngleAxis(Angle, ArrowComp->GetUpVector()) };
		End *= 250;
		End += PawnLoc;

		// DEBUG
		// DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 5);

		GetWorld()->LineTraceSingleByChannel(Result, Start, End, ECollisionChannel::ECC_Visibility, CollisionParams);

		if (Result.bBlockingHit)
			return true;
	}
	return false;
}

float APlayerPawn::FindAvailableDistanceUnderPawn()
{
	FVector Start { GetActorLocation() };
	// Ensure we don't begin the line trace into the sphere (which radius is 32)
	Start.X += 50;

	FVector End{ Start.X, Start.Y, Start.Z - MAX_ALTITUDE_IN_LEVEL };

	FHitResult Result{};
	FCollisionQueryParams CollisionParams;

	// DEBUG
	// DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 5);

	GetWorld()->LineTraceSingleByChannel(Result, Start, End, ECollisionChannel::ECC_Visibility, CollisionParams);

	return Result.Distance - 100.0f;
}
// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
/*
void APlayerPawn::Move(const FVector& World, float scale)
{

	if (CollisionQueryAlongXYAxis())
	{
		FVector PawnLoc{ GetActorLocation() };

		do
		{
			PawnLoc.Z += 500;
			SetActorLocation(PawnLoc, true);

		} while (CollisionQueryAlongXYAxis());
	}

	FVector Direction{};

	if (World == FVector::ForwardVector)
		Direction = ArrowComp->GetForwardVector();

	if (World == FVector::BackwardVector)
		Direction = ArrowComp->GetForwardVector() * -1;

	if (World == FVector::RightVector)
		Direction = ArrowComp->GetRightVector();

	if (World == FVector::LeftVector)
		Direction = ArrowComp->GetRightVector() * -1;

	Direction.Z = 0;
	AddMovementInput(Direction, scale, false);

	// check if pawn is allready at RequiredZLocation and if not, try to set it to
	if (RequiredZLocation != GetActorLocation().Z)
		TryMovePawnAtRequiredZLocation();
}
*/
void APlayerPawn::Move(bool bForwardAxis, float Scale)
{
	if (CollisionQueryAlongXYAxis())
	{
		FVector PawnLoc{ GetActorLocation() };

		do
		{
			PawnLoc.Z += 500;
			SetActorLocation(PawnLoc, true);

		} while (CollisionQueryAlongXYAxis());
	}

	FVector Direction{};

	if (bForwardAxis)
		Direction = ArrowComp->GetForwardVector() * Scale;
	else
		Direction = ArrowComp->GetRightVector() * Scale;

	SetActorLocation(GetActorLocation() + Direction, true);

	// check if pawn is allready at RequiredZLocation and if not, try to set it to
	if (RequiredZLocation != GetActorLocation().Z)
		TryMovePawnAtRequiredZLocation();

}

void APlayerPawn::Zoom(float ZoomScale)
{
	const FVector PawnLocation { GetActorLocation() };
	const FVector DirectionXY { SpringArmComp->GetForwardVector() * ZoomScale };

	FVector NewLocation { (DirectionXY * ZoomUnits) + PawnLocation};
	// Set XY location but not Z
	SetActorLocation(FVector(NewLocation.X, NewLocation.Y, PawnLocation.Z), true);
	// Set Z location
	TryMovePawnAtRequiredZLocation();
}

void APlayerPawn::AddSpringArmPitchRotation(float Angle, float MinPitchAngle, float MaxPitchAngle)
{
	FRotator Rotation{ SpringArmComp->GetRelativeRotation() };

	Rotation.Add(Angle, 0.0f, 0.0f);

	Rotation.Pitch = FMath::ClampAngle(Rotation.Pitch, MinPitchAngle, MaxPitchAngle);

	SpringArmComp->SetRelativeRotation(Rotation);
}

void APlayerPawn::SetSpringArmPitchRotation(float NewAngle, float MinPitchAngle, float MaxPitchAngle)
{
	NewAngle = FMath::ClampAngle(NewAngle, MinPitchAngle, MaxPitchAngle);

	SpringArmComp->SetRelativeRotation(FRotator(NewAngle, 0, 0));
}

float APlayerPawn::GetSpringArmPitchRotation()
{
	return SpringArmComp->GetRelativeRotation().Pitch;
}

void APlayerPawn::AddArrowComponentYawRotation(float Angle)
{
	FRotator Rotation{ ArrowComp->GetRelativeRotation() };
	Rotation.Add(0.0f, Angle, 0.0f);
	ArrowComp->SetRelativeRotation(Rotation);
}

void APlayerPawn::TryMovePawnAtRequiredZLocation()
{
	FVector PawnLoc{ GetActorLocation() };
	float AvailableDistance = FindAvailableDistanceUnderPawn();
	// If we can move the pawn
	if (RequiredZLocation >= PawnLoc.Z - AvailableDistance)
		PawnLoc.Z = RequiredZLocation;

	else
		PawnLoc.Z -= AvailableDistance;

	SetActorLocation(PawnLoc, true);
}


