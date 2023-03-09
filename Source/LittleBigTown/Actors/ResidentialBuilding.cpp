// Fill out your copyright notice in the Description page of Project Settings.


#include "ResidentialBuilding.h"
#include "LittleBigTown/UserInterface/BuildingInfos/UI_BuildingInfos.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Algo/Sort.h"
#include "LittleBigTown/GameSystem/MainGameState.h"
#include "LittleBigTown/Core/Debugger.h"
#include "LittleBigTown/Core/SocialClass.h"
#include "LittleBigTown/Actors/Pedestrian.h"

#define MAX_LEVEL_RESIDENTIAL 5
#define SPAWN_COOR_Z 14.0f
// #define TEN_PERCENT_OCCUPATION FMath::RoundToInt ((InfosBase.m_OccupationMaxCount * 10) / 100)

void AResidentialBuilding::BeginPlay()
{
	Super::BeginPlay();

	MainGameState = Cast <AMainGameState>(UGameplayStatics::GetGameState(GetWorld()));
	MainGameState->OnNewHourDelegate.AddDynamic(this, &ThisClass::UpdateNewHour);

	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ThisClass::TimerSpawnPedestrian, 5.0f, true, 1.0f);

	_SpawnTarget = GetActorLocation() + (_DistancePedestrianSpawn * StaticMeshComponent->GetForwardVector());


#ifdef DEBUG_ONLY

	checkf(InfosBase.m_OccupationMaxCount >= 5, TEXT("Error in AResidentialBuilding::BeginPlay : Minimum d'habitants requis = 5."));
	checkf(_DistancePedestrianSpawn >= 0, TEXT("Error in AResidentialBuilding::BeginPlay : DistancePedestrianSpawn <0."));

#endif 

}

void AResidentialBuilding::GenerateResidents(int32 Count)
{

#ifdef DEBUG_ONLY

	checkf(Count > 0, TEXT("Error in AResidentialBuilding::GenerateResidents : Count <= 0."));

#endif 

	const auto Frequencies{ MainGameState->GetSpecialtiesFrequencies(InfosBase.m_WealthLevel) };
	TEnumAsByte <ECitySpecialty> Specialty{ ECitySpecialty::DefaultCitySpecialtyEnum };
	TArray<int> Arr{};

	CreateRandTable(Arr, Frequencies);

	for (int i{ 0 }; i < Count; ++i)
	{
		int32 x{ FMath::RandRange(1, Arr[Arr.Num() - 1]) };

		auto It{ Frequencies.CreateConstIterator() };

		if (x > 0 && x <= Arr[0])
			Specialty = It->Key;
		else
		{
			for (int j{ 1 }; j < Arr.Num(); ++j)
			{
				++It;

				if (x > Arr[j - 1] && x <= Arr[j])
					Specialty = It->Key;
			}
		}

		EmplaceResident(Specialty);
		MainGameState->AddOrSubResidents(InfosBase.m_WealthLevel, Specialty, 1, ResidentialInformations.m_IncomesPerHab);
	}
}
/*
void AResidentialBuilding::GenerateResidents(int32 Count)
{

#ifdef DEBUG_ONLY

	checkf(Count > 0, TEXT("Error in AResidentialBuilding::GenerateResidents : Count <= 0."));

#endif 

	const auto Frequencies { MainGameState->GetSpecialtiesFrequencies(InfosBase.m_WealthLevel) };
	TEnumAsByte <ECitySpecialty> Specialty{ ECitySpecialty::DefaultCitySpecialtyEnum };
	TArray<int> Arr{};
	Arr.Init(0, Frequencies.Num());
	auto It { Frequencies.CreateConstIterator()};
	Arr[0] = FMath::RoundToInt(It->Value * 100) + 100;

	for (int i{ 1 }; i < Arr.Num(); ++i)
		Arr[i] = (FMath::RoundToInt((++It)->Value * 100) + 100) + Arr[i -1];

	for (int i{ 0 }; i < Count; ++i)
	{
		int32 x { FMath::RandRange(1, Arr[Arr.Num() - 1])};

		// Cannot res It
		auto NewIt{ Frequencies.CreateConstIterator() };

		if (x > 0 && x <= Arr[0])
			Specialty = NewIt->Key;
		else
		{
			for (int j{ 1 }; j < Arr.Num(); ++j)
			{
				++NewIt;

				if(x > Arr[j - 1] && x <= Arr[j])
					Specialty = NewIt->Key;
			}
		}

		// m_Occupants.Add(FResident(Specialty, MainGameState->GetSocialClasses(InfosBase.m_WealthLevel)));

		// Classer les Résidents par Satisfaction croissante lorsqu'on les ajoutes au tableau
		// On suppose ici que la Satisfaction par défaut des Résidents est de 0.50f
		int32 I { m_Occupants.Num() - 1 };

		while (I >= 0 && m_Occupants[I].m_Satisfaction > 0.50f)
			--I;

		FResident Resident { FResident(Specialty, MainGameState->GetSocialClasses(InfosBase.m_WealthLevel)) };
		I == m_Occupants.Num() - 1 ? m_Occupants.Add(Resident) : m_Occupants.EmplaceAt(I, Resident);

		MainGameState->AddOrSubResidents(InfosBase.m_WealthLevel, Specialty, 1, ResidentialInformations.m_IncomesPerHab);
	}
}

void AResidentialBuilding::RemoveResidents(int32 Count)
{
#ifdef DEBUG_ONLY

	checkf(Count < 0, TEXT("Error in AResidentialBuilding::RemoveResidents : Count < 0."));

#endif 

	// Sort by Satisfaction : Lowers are at Lowers Index
	Algo::Sort(m_Occupants, [](const FResident& Curr, const FResident& Next) -> bool {
		return Curr.m_Satisfaction < Next.m_Satisfaction; });

	for (int32 i { 0 }; i  < Count * (-1); i ++)
		MainGameState->AddOrSubResidents(InfosBase.m_WealthLevel, m_Occupants[0].m_Type, -1, ResidentialInformations.m_IncomesPerHab);

	m_Occupants.RemoveAt(0, Count * (-1), true);
}
*/
void AResidentialBuilding::RemoveResidents(int32 Count)
{
#ifdef DEBUG_ONLY

	checkf(Count < 0, TEXT("Error in AResidentialBuilding::RemoveResidents : Count < 0."));
	checkf(m_Occupants.Num() > 0, TEXT("Error in AResidentialBuilding::RemoveResidents, m_Occupants.Num() == 0."));

#endif 

	if (Count >= 0 || m_Occupants.Num() == 0)
		return;

	// Si EmplaceResident() a bien été appelée au sein de GenerateResidents(), 
	// ces derniers sont déjà classés par ordre croissants de Satisfaction
	for (int32 i{ 0 }; i < Count * (-1); i++)
		MainGameState->AddOrSubResidents(InfosBase.m_WealthLevel, m_Occupants[0].m_Type, -1, ResidentialInformations.m_IncomesPerHab);

	m_Occupants.RemoveAt(0, Count * (-1), true);
}

void AResidentialBuilding::CreateRandTable(TArray <int32>& Arr, const TMap<ECitySpecialty, float>& Freq)
{
	Arr.Init(0, Freq.Num());
	auto It { Freq.CreateConstIterator() };

	Arr[0] = FMath::RoundToInt(It->Value * 100) + 100;

	for (int i{ 1 }; i < Arr.Num(); ++i)
		Arr[i] = (FMath::RoundToInt((++It)->Value * 100) + 100) + Arr[i - 1];
}

void AResidentialBuilding::EmplaceResident(const TEnumAsByte <ECitySpecialty>& Specialty)
{
	FResident Resident{ FResident(Specialty, MainGameState->GetSocialClasses(InfosBase.m_WealthLevel)) };
	
	// Last() plante si Array vide ...
	if (m_Occupants.Num() == 0)
		m_Occupants.Add(Resident);

	else if (m_Occupants.Last().m_Satisfaction <= 0.5f)
		m_Occupants.Add(Resident);

	else
	{
		int32 I { m_Occupants.FindLastByPredicate([] (const FResident& Res)-> bool { return Res.m_Satisfaction <= 0.5f; }) };

		if (I != INDEX_NONE)
			m_Occupants.EmplaceAt(I, Resident);

#ifdef DEBUG_ONLY

		checkf(I != INDEX_NONE, TEXT("Error in AResidentialBuilding::EmplaceResident, I == INDEX_NONE."));

#endif

	}
}

void AResidentialBuilding::UpdateNewHour(int32 Hour)
{
	double Demand { MainGameState->GetResidentialDemand(InfosBase.m_WealthLevel) };
	const int32 Constant {FMath::Clamp(FMath::RoundToInt((InfosBase.m_OccupationMaxCount * 10) / 100), 1, 500)};
	int32 OccupationVar { FMath::RoundToInt((Demand * Constant) + ((ResidentialInformations.m_SatisfactionPercent - 0.5) * Constant)) };

	// Ne pas enlever plus d'habitants qu'il n'y en a ou ne pas en ajouter plus que le logement ne peut en contenir
	OccupationVar = FMath::Clamp(OccupationVar, -InfosBase.m_OccupationCurrentCount, InfosBase.m_OccupationMaxCount - InfosBase.m_OccupationCurrentCount);

	// Modification des infos de l'habitation :
	InfosBase.m_OccupationCurrentCount += OccupationVar;
	ResidentialInformations.m_TotalIncomes += OccupationVar * ResidentialInformations.m_IncomesPerHab;

	if (OccupationVar > 0)
		GenerateResidents(OccupationVar);

	else if (OccupationVar < 0)
		RemoveResidents(OccupationVar);

	else
		return;

	OnResBuildingInfosChangedDelegate.Broadcast();
}

void AResidentialBuilding::AddSatisfaction(float Satisfaction)
{
/*
#ifdef DEBUG_ONLY

	checkf(Satisfaction >= -1.0f && Satisfaction <= 1.0f, 
		TEXT("Error in AResidentialBuilding::SetSatisfaction : Satisfaction has'nt a valid value."));

#endif 

*/
	ResidentialInformations.m_SatisfactionPercent = FMath::Clamp(ResidentialInformations.m_SatisfactionPercent + Satisfaction, 0.0f, 1.0f);

	// Declare Building Infos Modification :
	OnResBuildingInfosChangedDelegate.Broadcast();
}
/*
// --------------------------------------------		WARNING : BLUEPRINT VERSION		--------------------------------------------
void AResidentialBuilding::GetSpawnableResident(FResident * ResidentPtr)
{
	if (m_Occupants.Num() != 0)
	{
		int32 I{ -1 };

		// Find Available Resident
		for (int32 i{ 0 }; i < m_Occupants.Num(); ++i)
		{
			if (!m_Occupants[i]._Pedestrian.IsValid())
			{
				I = i; break;
			}
		}

		// PrintString(FString::Printf(TEXT("%d"), I));

		// No available Resident
		if (I == -1)
			return;

		ResidentPtr = &m_Occupants[I];
	}
}
*/

void AResidentialBuilding::TimerSpawnPedestrian()
{
	if (m_Occupants.Num() != 0)
	{
		int32 I { -1 };

		// Find Available Resident
		for (int32 i{ 0 }; i < m_Occupants.Num(); ++i)
		{
			if (!m_Occupants[i]._Pedestrian.IsValid())
			{
				I = i; break;
			}
		}

		// PrintString(FString::Printf(TEXT("%d"), I));

		// Maybye We Should Stop Timer Here
		if (I == -1)
			return;

		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;

		auto Actor { GetWorld()->SpawnActor <APedestrian> (_SpawnTarget, StaticMeshComponent->GetComponentRotation(), SpawnInfo)};

		if (IsValid(Actor))
		{
			// Cast <APedestrian>(Actor)->Init(&m_Occupants[I], this, true);

			Cast <APedestrian>(Actor)->Init(this, true);

			// Set Weak Object Ref into FResident
			m_Occupants[I]._Pedestrian = Cast <APedestrian>(Actor);
		}
	}
}