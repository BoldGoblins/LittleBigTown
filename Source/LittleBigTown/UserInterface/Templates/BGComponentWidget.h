// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BGComponentWidget.generated.h"

UCLASS(Abstract)
class LITTLEBIGTOWN_API UBGComponentWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual UWidget* GetWrappedWidget();
	
};
