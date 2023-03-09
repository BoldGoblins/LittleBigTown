// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"


#include "BTTask_FindRandomLocation.generated.h"


UCLASS()
class LITTLEBIGTOWN_API UBTTask_FindRandomLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public : 

	UBTTask_FindRandomLocation();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	FString GetStaticDescription() const override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float _SearchRadius { 500.0f };
};
