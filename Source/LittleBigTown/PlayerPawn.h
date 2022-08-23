// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

UCLASS()
class LITTLEBIGTOWN_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FVector& World, float scale);

	// Assume scale is already clamped (1-10) before coming as argument.
	void Zoom(float scale);

protected : 

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class UCameraComponent* CameraComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class USpringArmComponent* SpringArmComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class USceneComponent* RootComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class USphereComponent* SphereComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class UFloatingPawnMovement* PawnMovement;

	UPROPERTY(BlueprintReadOnly)
		class APlayerController* PlayerController;


};
