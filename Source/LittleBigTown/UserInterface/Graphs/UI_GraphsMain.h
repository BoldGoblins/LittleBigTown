// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_GraphsMain.generated.h"

/**
 * 
 */
UCLASS()
class LITTLEBIGTOWN_API UUI_GraphsMain : public UUserWidget
{
	GENERATED_BODY()

public : 

	void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
		void DisplayGraph(int32 Index);

	UFUNCTION(BlueprintCallable)
		void HideWidget();


protected : 

	class UUI_GraphDemand* GraphDemand;

	UPROPERTY(Meta = (BindWidget))
		class UWidgetSwitcher* WidgetSwitcher;
	
};
