// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BGUserWidget.generated.h"

// Base Class for all Widgets in the Game
UCLASS(Abstract)
class LITTLEBIGTOWN_API UBGUserWidget : public UUserWidget
{
	GENERATED_BODY()

public : 

	// Default Implémentation

	UFUNCTION(BlueprintCallable)
		virtual void Hide();
	// virtual void Reset() ?
};
