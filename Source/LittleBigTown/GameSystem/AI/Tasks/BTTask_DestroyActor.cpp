// Fill out your copyright notice in the Description page of Project Settings.


#include "LittleBigTown/GameSystem/AI/Tasks/BTTask_DestroyActor.h"

#include "LittleBigTown/Actors/Pedestrian.h"
#include "LittleBigTown/Core/Resident.h"
#include "LittleBigTown/Core/Debugger.h"

#include "AIController.h"

UBTTask_DestroyActor::UBTTask_DestroyActor()
{
	NodeName = TEXT("Destroy Actor");
}

EBTNodeResult::Type UBTTask_DestroyActor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// AI Controller 
	const auto AIC{ OwnerComp.GetAIOwner() };
	const auto Pedestrian { Cast <APedestrian> (AIC->GetPawn()) };

#ifdef DEBUG_ONLY

	UE_LOG(LogTemp, Warning, TEXT("Pedestrian Destroyed from TaskNode n° : %d"), *NodeMemory);

#endif
	/*
	if (IsValid(Pedestrian) && Pedestrian->GetResident())
	{
		Pedestrian->GetResident()->_Pedestrian.Reset();
		Pedestrian->Destroy();
	}
	*/

	if (IsValid(Pedestrian))
		Pedestrian->Destroy();

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

FString UBTTask_DestroyActor::GetStaticDescription() const
{
	return FString::Printf(TEXT("Destroy the Actor, Notifies FResident if exist"));
}
