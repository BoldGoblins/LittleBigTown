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
	/*
	InfosBase.m_Name = Infos.m_Name;
	InfosBase.m_CurrentLevel = Infos.m_CurrentLevel;
	InfosBase.m_MaxLevel = Infos.m_MaxLevel;
	InfosBase.m_OccupationMaxCount = Infos.m_OccupationMaxCount;
	InfosBase.m_OccupationCurrentCount = Infos.m_OccupationCurrentCount;
	InfosBase.m_Outgoings = Infos.m_Outgoings;
	*/
}


