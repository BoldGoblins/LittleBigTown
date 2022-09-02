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
// Player Controller header
#include "MainPlayerController.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Get Player Controller Ref
	PlayerController = Cast <AMainPlayerController> (UGameplayStatics::GetPlayerController(GetWorld(), 0));
	
	// Construction hidden Static Mesh
	SphereComp = CreateDefaultSubobject <USphereComponent>(TEXT("Hidden Sphere"));
	SetRootComponent(SphereComp);
	
	// Construction SpringArm
	SpringArmComp = CreateDefaultSubobject <USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->AttachToComponent(SphereComp, FAttachmentTransformRules::KeepWorldTransform);
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

	// Value SpringArm related that needs SpringArm to be initialized with the good value.
	ZoomUnits = SpringArmComp->TargetArmLength / PlayerController->GetZoomMin();
	ZoomInterpSpeed = ZoomUnits / 100;
	CamZoomDestination = SpringArmComp->TargetArmLength;
	// Set default rotation for Spring Arm 
	SpringArmComp->SetRelativeRotation(FRotator(DEFAULT_PITCH_ROTATION_PAWN, 0.0f, 0.0f));
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SpringArmComp->TargetArmLength != CamZoomDestination)
		SpringArmComp->TargetArmLength = FMath::FInterpTo(SpringArmComp->TargetArmLength, CamZoomDestination, DeltaTime, ZoomInterpSpeed);
	
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerPawn::Move(const FVector& World, float scale)
{
	FVector Direction{};
	
	if (World == FVector::ForwardVector)
		Direction = GetActorForwardVector();

	if (World == FVector::BackwardVector)
		Direction = GetActorForwardVector () * -1;

	if (World == FVector::RightVector)
		Direction = GetActorRightVector();

	if (World == FVector::LeftVector)
		Direction = GetActorRightVector() * -1;

	Direction.Z = 0;
	AddMovementInput(Direction, scale, false);
}

void APlayerPawn::Zoom (int scale)
{
	CamZoomDestination = ZoomUnits * scale;
}

void APlayerPawn::SpringArmPitchRotation(const FRotator& Rotation)
{	
	SpringArmComp->SetRelativeRotation(Rotation);
}

