// Fill out your copyright notice in the Description page of Project Settings.


#include "LittleBigTown/Core/Resident.h"
#include "LittleBigTown/Core/Debugger.h"

FResident::~FResident()
{
	/*
#ifdef DEBUG_ONLY

	UE_LOG(LogTemp, Warning, TEXT("FResident DestructorCalled."));

#endif

	if (_Pedestrian.IsValid())
	{
		bool IsDestroyed { _Pedestrian->Destroy() };

#ifdef DEBUG_ONLY

		checkf(IsDestroyed, TEXT("Error in Destructor of Resident, Can't Destroy _Pedestrian."));
#endif

		_Pedestrian.Reset();
	}
	*/
}
