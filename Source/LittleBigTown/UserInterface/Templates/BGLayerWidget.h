// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LittleBigTown/UserInterface/Templates/BGUserWidget.h"

#include "BGLayerWidget.generated.h"

UCLASS(Abstract)
class LITTLEBIGTOWN_API UBGLayerWidget : public UBGUserWidget
{
	GENERATED_BODY()
	
public : 

	virtual void SetInformations(class ABuilding* Building, bool bNewDisplay = true);

private:

	// delete Display
	using UBGUserWidget::Display;
	// delete Hide
	using UBGUserWidget::Hide;
	// delete Update
	using UBGUserWidget::Update;
};
