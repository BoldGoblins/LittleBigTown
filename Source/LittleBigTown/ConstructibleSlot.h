// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LittleBigTown.h"

#include "ConstructibleSlot.generated.h"


UCLASS(Abstract)
class LITTLEBIGTOWN_API AConstructibleSlot : public AActor
{
	GENERATED_BODY()
	
public:

	// Sets default values for this actor's properties
	AConstructibleSlot();

	// Bind to OnClickedEvent Delegate
	// Call OnSlotClickedDelegate from MainPlayerController
	UFUNCTION()
		void OnSlotClicked(AActor* Target, FKey ButtonPressed);

	// Return SlotType of this ConstructibleSlot
	UFUNCTION(BlueprintCallable, BlueprintPure)
		TEnumAsByte <ESlotType> GetSlotType() { return SlotType; }

	// Return ConstructionTheme of this ConstructibleSlot
	UFUNCTION(BlueprintCallable, BlueprintPure)
		TMap <FName, bool> GetConstructionThemes() { return ConstructionThemes; }

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Type of the Slot (Enum defined in LittleBigTown.h)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TEnumAsByte <ESlotType> SlotType { ESlotType::DefaultEnum };

	// Map used to activate/deactivate Thematic Buttons
	// According to what kind of Buildings Themes are availables for a specific Slot
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TMap <FName, bool> ConstructionThemes;

	class AMainPlayerController* MainPlayerControllerRef {};

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
