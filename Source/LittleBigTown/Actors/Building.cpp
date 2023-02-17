// Fill out your copyright notice in the Description page of Project Settings.


#include "Building.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Algo/Copy.h"


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

}

const TArray<FResident> ABuilding::GetOccupants(const ECitySpecialty& Specialty) const
{
	TArray <FResident> ToRet {};
	
	for (const auto& Resident : m_Occupants)
	{
		if (Resident.m_Type == Specialty)
			ToRet.Add(Resident);
	}

	return ToRet;
}

const TArray<FResident> ABuilding::GetOccupants(const FString& SubClassName) const
{
	TArray <FResident> Copy{};

	Algo::CopyIf(m_Occupants, Copy, [&SubClassName](const FResident& Resident)-> bool {
		return Resident.m_SubClassName.ToString() == SubClassName; });

	return Copy;
}


