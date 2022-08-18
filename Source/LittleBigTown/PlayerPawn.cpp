// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
// UCameraComponent
#include "Camera/CameraComponent.h"
//
#include "Components/ArrowComponent.h"
// USpringArmComponent
#include "GameFramework/SpringArmComponent.h"
// RootComponent
#include "Components/SceneComponent.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Construction RootComp
	RootComp = CreateDefaultSubobject <USceneComponent>(TEXT("Root"));

	// Construction SpringArm
	SpringArmComp = CreateDefaultSubobject <USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComp);
	// Construction Camera
	CameraComp = CreateDefaultSubobject <UCameraComponent>(TEXT("Camera"));
	CameraComp->AttachToComponent(SpringArmComp, FAttachmentTransformRules::KeepRelativeTransform);


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

