// Fill out your copyright notice in the Description page of Project Settings.


#include "LittleBigTown/GameSystem/AI/Tasks/BTTask_FindHomeLocation.h"

#include "LittleBigTown/Actors/Pedestrian.h"
#include "LittleBigTown/Actors/ResidentialBuilding.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindHomeLocation::UBTTask_FindHomeLocation()
{
	NodeName = TEXT("FindHomeLocation");

	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindHomeLocation, BlackboardKey));
}

EBTNodeResult::Type UBTTask_FindHomeLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// AI Controller 
	const auto AIC{ OwnerComp.GetAIOwner() };

	// Get AI Pawn
	const auto Pedestrian { Cast <APedestrian> (AIC->GetPawn()) };

	if (!IsValid(Pedestrian) || !Pedestrian->GetOrigin().IsValid())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}

	AIC->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, Pedestrian->GetOrigin().Get()->GetSpawnTarget());
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		
	return EBTNodeResult::Succeeded;
}

FString UBTTask_FindHomeLocation::GetStaticDescription() const
{
	return FString::Printf(TEXT("Vector : %s"), *BlackboardKey.SelectedKeyName.ToString());
}
