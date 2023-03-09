// Fill out your copyright notice in the Description page of Project Settings.


#include "LittleBigTown/Actors/Pedestrian.h"

#include "Components/ArrowComponent.h"
//v#include "Components/CapsuleComponent.h"
#include "LittleBigTown/Core/Resident.h"
#include "LittleBigTown/Actors/ResidentialBuilding.h"
#include "LittleBigTown/Core/Debugger.h"
#include "LittleBigTown/GameSystem/AI/AICPedestrian.h"
#include "GameFramework/FloatingPawnMovement.h"

// #define CAPSULE_RADIUS 15.0f
// #define CAPSULE_HALF_HEIGH 50.0f
#define MESH_OFFSET_Z -85.0f
#define PAWN_MAX_SPEED 100.0f
#define MAX_CULLING_DISTANCE 5000.0f


void APedestrian::Init (AResidentialBuilding* Origin, bool bIsMale)
{

#ifdef DEBUG_ONLY

	// checkf(Resident, TEXT("Error in APedestrian Constructor, Resident nullptr."));
	checkf(Origin, TEXT("Error in APedestrian Constructor, Origin nullptr."));

	UE_LOG(LogTemp, Warning, TEXT("Pedestrian Init ! "));

#endif

	// _Resident = Resident;
	_Origin = Origin;
	_Name = FText::FromString(TEXT("NameTest"));

	if (bIsMale)
		_SkeletalMeshComp->SetSkeletalMesh(MaleSkeletal);
	else
		_SkeletalMeshComp->SetSkeletalMesh(FemaleSkeletal);
	
}


/*
// --------------------------------------------		WARNING : BLUEPRINT VERSION		--------------------------------------------
void APedestrian::Init(AResidentialBuilding* Origin, bool bIsMale)
{

#ifdef DEBUG_ONLY

	checkf(IsValid(Origin), TEXT("Error in APedestrian Constructor, Origin nullptr."));

	UE_LOG(LogTemp, Warning, TEXT("Pedestrian Init ! "));

#endif

	if (!IsValid(Origin))
		return; 

	_Origin = Origin;
	_Origin.Get()->GetSpawnableResident(_Resident);
	// _Resident->_Pedestrian = this;
	_Name = FText::FromString(TEXT("NameTest"));

	if (bIsMale)
		_SkeletalMeshComp->SetSkeletalMesh(MaleSkeletal);
	else
		_SkeletalMeshComp->SetSkeletalMesh(FemaleSkeletal);

}
*/
APedestrian::APedestrian()
{
#ifdef DEBUG_ONLY

	UE_LOG(LogTemp, Warning, TEXT("Pedestrian Construct ! "));

#endif

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// bReplicates = true;

	// Enable AI Controller to Steer the Pawn Rotation
	bUseControllerRotationYaw = true;

	_ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));

	RootComponent = _ArrowComp;
	_ArrowComp->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	_ArrowComp->SetArrowColor(FLinearColor::Red);


	_SkeletalMeshComp = CreateDefaultSubobject <USkeletalMeshComponent>(TEXT("SK_Component"));

	_SkeletalMeshComp->SetupAttachment(_ArrowComp);
	_SkeletalMeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, MESH_OFFSET_Z));
	_SkeletalMeshComp->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	_SkeletalMeshComp->SetWorldScale3D(FVector(0.4f));
	_SkeletalMeshComp->SetCastShadow(_SkeletalMeshComp->CastShadow = false);
	_SkeletalMeshComp->SetEnableGravity(false);
	// Culling
	_SkeletalMeshComp->bNeverDistanceCull = false;
	_SkeletalMeshComp->SetCachedMaxDrawDistance(MAX_CULLING_DISTANCE);

	static ConstructorHelpers::FObjectFinder <USkeletalMesh> SkeletalMeshMale { TEXT("/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin") };

	static ConstructorHelpers::FObjectFinder <USkeletalMesh> SkeletalMeshFemale{ TEXT("/Game/Mannequin/Character/Mesh/SK_Mannequin_Female.SK_Mannequin_Female") };

	if (SkeletalMeshMale.Succeeded() && SkeletalMeshFemale.Succeeded())
	{
		MaleSkeletal = SkeletalMeshMale.Object;
		FemaleSkeletal = SkeletalMeshFemale.Object;
	}

	// AI Controller : 
	AIControllerClass = AAICPedestrian::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	_PawnMovComp = CreateDefaultSubobject<UFloatingPawnMovement> (TEXT("PawnMovComp"));

	_PawnMovComp->MaxSpeed = PAWN_MAX_SPEED;
	_PawnMovComp->Acceleration = _PawnMovComp->MaxSpeed / 100.0f;
	_PawnMovComp->Deceleration = _PawnMovComp->Acceleration;

#ifdef DEBUG_ONLY

	_ArrowComp->bHiddenInGame = false;
	_ArrowComp->SetVisibility(true);

#endif

}

// Called when the game starts or when spawned
void APedestrian::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APedestrian::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
/*
// Called to bind functionality to input
void APedestrian::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
*/

