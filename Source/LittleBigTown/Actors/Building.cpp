// Fill out your copyright notice in the Description page of Project Settings.


#include "Building.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABuilding::ABuilding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject <USceneComponent> (TEXT("RootComponent"));
	SetRootComponent(RootComp);
	RootComp->SetMobility(EComponentMobility::Static);
	StaticMeshComponent = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->AttachToComponent(RootComp, FAttachmentTransformRules::KeepRelativeTransform);
	StaticMeshComponent->SetCollisionProfileName(TEXT("Buildings"));
	StaticMeshComponent->SetMobility(EComponentMobility::Static);

	Tags.Add(TEXT("Building"));
}

void ABuilding::BeginPlay()
{
	Super::BeginPlay();

	InfosBase.m_OccupationCurrentCount = 0;
	InfosBase.m_Outgoings = Current_Outgoings;
	InfosBase.m_WealthLevel = WealthLevel;
	InfosBase.m_Name = Name;
	InfosBase.m_Description = Description;
	InfosBase.m_CurrentLevel = CurrentLevel;
}


