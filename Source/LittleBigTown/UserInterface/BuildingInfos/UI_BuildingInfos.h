// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
// struct residential infos definition
#include "LittleBigTown/Core/Structs.h"

#include "UI_BuildingInfos.generated.h"

// FText MakeProportionText(int32 a, int32 b);

#define WIDGET_GENERAL_POS 0


UCLASS()
class LITTLEBIGTOWN_API UUI_BuildingInfos : public UUserWidget
{
	GENERATED_BODY()


public : 

	void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
		void NewDisplayBuildingInfos(class ABuilding* Building);

	UFUNCTION(BlueprintCallable)
		void HideBuildingInfo();

	UFUNCTION()
		void BuildingInfosUpdated();


protected :

	TWeakObjectPtr <class AResidentialBuilding> CurrResBuilding;

	class UUI_General_Infos* GeneralInfosWidget;

	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* WidgetSwitcher;

	// --------------------------------------		TEXTBLOCKS		--------------------------------------


	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TB_Name;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TB_Owner;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TB_District;

};
