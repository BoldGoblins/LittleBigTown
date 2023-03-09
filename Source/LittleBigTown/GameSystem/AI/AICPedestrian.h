// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "AICPedestrian.generated.h"


UCLASS()
class LITTLEBIGTOWN_API AAICPedestrian : public AAIController
{
	GENERATED_BODY()

public : 

	AAICPedestrian();

	void BeginPlay() override;

	void OnPossess(APawn* InPawn) override;


protected :

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class UBehaviorTreeComponent* BehaviorTreeComponent;

	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		// class UBlackboardComponent* BlackboardComponent;

	
};
