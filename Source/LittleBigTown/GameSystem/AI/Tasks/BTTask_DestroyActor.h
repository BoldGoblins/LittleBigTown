// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_DestroyActor.generated.h"


UCLASS()
class LITTLEBIGTOWN_API UBTTask_DestroyActor : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public : 

	UBTTask_DestroyActor();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	FString GetStaticDescription() const override;
	
};
