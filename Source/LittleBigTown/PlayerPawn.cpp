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

#include "Components/ArrowComponent.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Get Player Controller Ref
	PlayerController = Cast <AMainPlayerController> (UGameplayStatics::GetPlayerController(GetWorld(), 0));
	
	// Construction hidden Static Mesh
	SphereComp = CreateDefaultSubobject <USphereComponent> (TEXT("Hidden Sphere"));
	SetRootComponent(SphereComp);

	// Construction ArrowComponent
	ArrowComp = CreateDefaultSubobject <UArrowComponent> (TEXT("Arrow Component"));
	ArrowComp->AttachToComponent(SphereComp, FAttachmentTransformRules::KeepRelativeTransform);

	// Construction SpringArm
	SpringArmComp = CreateDefaultSubobject <USpringArmComponent> (TEXT("SpringArm"));
	SpringArmComp->AttachToComponent(ArrowComp, FAttachmentTransformRules::KeepRelativeTransform);
	SpringArmComp->TargetArmLength = DEFAULT_SPRING_ARM_LENGTH;

	// Construction Camera
	CameraComp = CreateDefaultSubobject <UCameraComponent> (TEXT("Camera"));
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

bool APlayerPawn::CollisionQuery()
{
	FVector Start { this->GetActorLocation() };
	FHitResult Result {};

	const FName TraceTag("MyTraceTag");
	GetWorld()->DebugDrawTraceTag = TraceTag;
	FCollisionQueryParams CollisionParams;
	CollisionParams.TraceTag = TraceTag;

	for (int i{ 0 }; i < 8; i++)
	{
		FVector Forward { ArrowComp->GetForwardVector()};
		Forward.RotateAngleAxis((360 / 8) * i, FVector(0,1,0));
		
		FVector End { (Forward * 800) + Start };
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%f"), Forward.Y));
		GetWorld()->LineTraceSingleByChannel(Result, Start, End, ECollisionChannel::ECC_Visibility, CollisionParams);
	}






	return false;
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
		Direction = ArrowComp->GetForwardVector(); 

	if (World == FVector::BackwardVector)
		Direction = ArrowComp->GetForwardVector() * -1;

	if (World == FVector::RightVector)
		Direction = ArrowComp->GetRightVector();

	if (World == FVector::LeftVector)
		Direction = ArrowComp->GetRightVector() * -1;

	Direction.Z = 0;
	AddMovementInput(Direction, scale, false);
	// CollisionQuery();
}

void APlayerPawn::Zoom (int scale)
{
	CamZoomDestination = ZoomUnits * scale;
}

void APlayerPawn::SpringArmPitchRotationByAxis (float Axis, float MinPitchAngle, float MaxPitchAngle)
{

	FRotator Rotation{ SpringArmComp->GetRelativeRotation() };

	Rotation.Add(Axis, 0.0f, 0.0f);
	Rotation.Pitch = FMath::ClampAngle(Rotation.Pitch, MinPitchAngle, MaxPitchAngle);
	SpringArmComp->SetRelativeRotation(Rotation);
}
void APlayerPawn::SpringArmPitchRotationByMaxAngle(float NewAngle, float Previous)
{

	FRotator Rotation{ SpringArmComp->GetRelativeRotation() };

	if (FMath::IsNearlyEqual(Rotation.Pitch, Previous, 0.5f))
	{
		Rotation.Pitch = NewAngle;
		SpringArmComp->SetRelativeRotation(Rotation);
	}
}

void APlayerPawn::ArrowComponentYawRotationByAxis(float Axis)
{
	FRotator Rotation{ ArrowComp->GetRelativeRotation() };
	Rotation.Add(0.0f, Axis, 0.0f);
	ArrowComp->SetRelativeRotation(Rotation);
}

