// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "UI_ConstructionValidation.generated.h"

UCLASS()
class LITTLEBIGTOWN_API UUI_ConstructionValidation : public UUserWidget
{
	GENERATED_BODY()

public : 

	void DisplayValidationWidget(const FName& Name);

	UFUNCTION(BlueprintCallable)
		const FText GetBuildingName() const;

protected : 

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* BuildingName;
	
};
