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

	OnClicked.AddUniqueDynamic(this, &ThisClass::OnBuildingClicked);
}

// Called when the game starts or when spawned
void ABuilding::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABuilding::OnBuildingClicked(AActor* Target, FKey ButtonPressed)
{

}

