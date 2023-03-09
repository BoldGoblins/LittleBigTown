// Fill out your copyright notice in the Description page of Project Settings.


#include "LittleBigTown/GameSystem/AI/AICPedestrian.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

// Pas besoin de créer un Blackboard, car on peut le récupérer directement depuis le BehaviorTree (si set en BP)
AAICPedestrian::AAICPedestrian()
{
	BehaviorTreeComponent = CreateDefaultSubobject <UBehaviorTreeComponent> (TEXT("BehaviorTreeComponent"));

	ConstructorHelpers::FObjectFinder <UBehaviorTree> BT { TEXT("/Game/GameSystem/AIController/BP_BT_Pedestrian.BP_BT_Pedestrian") };

	if (BT.Succeeded())
		BehaviorTree = BT.Object;
}

void AAICPedestrian::BeginPlay()
{
	Super::BeginPlay();

	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
		BehaviorTreeComponent->StartTree(*BehaviorTree);
	}
}

void AAICPedestrian::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (IsValid(Blackboard) && IsValid(BehaviorTree))
		Blackboard->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
}
