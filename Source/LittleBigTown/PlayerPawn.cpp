// Fill out your copyright notice in the Description page of Project Settings.

// For definitions
#include "LittleBigTown.h"

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
// 
#include "MainPlayerController.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Get Player Controller Ref
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	// Construction RootComp
	RootComp = CreateDefaultSubobject <USceneComponent>(TEXT("Root"));

	// Construction hidden Static Mesh
	SphereComp = CreateDefaultSubobject <USphereComponent>(TEXT("Hidden Sphere"));
	SphereComp->SetupAttachment(RootComp);

	// Construction SpringArm
	SpringArmComp = CreateDefaultSubobject <USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(SphereComp);
	SpringArmComp->TargetArmLength = DEFAULT_SPRING_ARM_LENGTH;
	// Test
	SpringArmComp->bUsePawnControlRotation = false;
	SpringArmComp->bEnableCameraLag = false;

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

void APlayerPawn::Move(const FVector& World, float scale)
{
	AddMovementInput(World, scale, false);
	//AddActorLocalOffset()
}

void APlayerPawn::Zoom (float scale)
{
	SpringArmComp->TargetArmLength = (ZOOM_SPEED * scale);
}
