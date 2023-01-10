// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LittleBigTown/Core/Enums.h"

#include "ConstructibleSlot.generated.h"


UCLASS(Abstract)
class LITTLEBIGTOWN_API AConstructibleSlot : public AActor
{
	GENERATED_BODY()
	
public:

	// Sets default values for this actor's properties
	AConstructibleSlot();

	// Return SlotType of this ConstructibleSlot
	UFUNCTION(BlueprintCallable, BlueprintPure)
		TEnumAsByte <ESlotType> GetSlotType() { return SlotType; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		TEnumAsByte <ESlotSize>	GetSlotSize() { return SlotSize; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FName GetDistrictName() { return SlotDistrictName; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		int GetSlotPrice() { return SlotPrice; }

	UFUNCTION(BlueprintCallable)
		void ModifySlotPriceBy(int Value) { SlotPrice += Value; }

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Type of the Slot (Enum defined in LittleBigTown.h)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TEnumAsByte <ESlotType> SlotType { ESlotType::DefaultTypeEnum };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TEnumAsByte <ESlotSize> SlotSize { ESlotSize::DefaultSizeEnum };

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int SlotPrice { 0 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName SlotDistrictName {"District"};

private :


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
