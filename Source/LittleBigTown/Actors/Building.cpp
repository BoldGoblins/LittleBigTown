// Fill out your copyright notice in the Description page of Project Settings.


#include "Building.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Algo/Copy.h"

#include "LittleBigTown/Core/Debugger.h"


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

float ABuilding::GetBuildingHeight() const
{
	FVector Origin{};
	FVector BoxHalfExtent{};
	GetActorBounds(false, Origin, BoxHalfExtent);

#ifdef DEBUG_ONLY

	// UE_LOG(LogTemp, Warning, TEXT("WARNING : ABuilding::GetBuildingHeight, Building Origin (Z Axis) != 0.0f."));
	// PrintString(TEXT("WARNING : ABuilding::GetBuildingHeight, Building Origin (Z Axis) != 0.0f."));
	// PrintString(FString::Printf(TEXT("Origin : %s, Extent : %s"), *Origin.ToString(), *BoxHalfExtent.ToString()));

#endif

	if (FMath::IsNearlyZero(Origin.Z, 0.0f))
		return BoxHalfExtent.Z * 2;

	else
		return (BoxHalfExtent.Z * 2) + Origin.Z;
}


