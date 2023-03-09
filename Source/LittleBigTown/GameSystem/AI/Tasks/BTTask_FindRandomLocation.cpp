// Fill out your copyright notice in the Description page of Project Settings.


#include "LittleBigTown/GameSystem/AI/Tasks/BTTask_FindRandomLocation.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "LittleBigTown/Core/Debugger.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation()
{
	// Display the Name of the Node in BP
	NodeName = TEXT("Find Random Location");

	// S'assurer qu'uniquement des Keys d'un certain type (ici Vector) peuvent être renseignées dans l'onglet "KeySelector"
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindRandomLocation, BlackboardKey));
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FNavLocation Location {};

	// AI Controller 
	const auto AIC { OwnerComp.GetAIOwner() };

	// Get AI Pawn
	const auto Pawn { AIC->GetPawn() };

	// Get Pawn Location
	const FVector Origin { Pawn->GetActorLocation() };

	const UNavigationSystemV1* NavSystem { UNavigationSystemV1::GetCurrent(GetWorld()) };

	// Obtenir les coordonnées et les envoyer au Blackboard en fonction du "Key Selector"
	// Key Selector est un onglet en BP que l'on renseigne avec la clé du Blackboard de notre choix
	// Récupérer la Key en C++ via BlackboardKey.SelectedKeyName
	if (IsValid(NavSystem) && NavSystem->GetRandomReachablePointInRadius(Origin, _SearchRadius, Location))
	{
		AIC->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, Location.Location);

		// Avertir le BehaviorTree Component que la tâche a été exécutée et qu'elle s'est terminée correctement
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	return EBTNodeResult::Failed;
}

// Permet d'afficher en temps réel dans l'Editeur le nom de la Key sélectionnée
FString UBTTask_FindRandomLocation::GetStaticDescription() const
{
	return FString::Printf(TEXT("Vector : %s"), *BlackboardKey.SelectedKeyName.ToString());
}
