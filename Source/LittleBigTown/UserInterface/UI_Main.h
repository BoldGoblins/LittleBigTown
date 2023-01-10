// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "UI_Main.generated.h"


#define BUILDING_INFOS_POS 1

UCLASS()
class LITTLEBIGTOWN_API UUI_Main : public UUserWidget
{
	GENERATED_BODY()

public : 

	UFUNCTION()
		void SetClockMinuteUpdate(int32 Minute);

	UFUNCTION()
		void SetClockHourUpdate(int32 Hour);

	UFUNCTION()
		void SetClockMonthUpdate(int32 Month);

	UFUNCTION()
		void SetClockYearUpdate(int32 Year);

	void NativeConstruct() override;

	void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		class UUI_BuildingInfos* GetBuildingInfosWidget() const { return BuildingInfosWidget; }

private : 
	
	void GamePropertiesDisplay();

protected :

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* ProgressBar;

	// Don't forget to set in Construct according to MainGameState.h #definitions
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TB_Time;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TB_Month;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TB_Year;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TB_Population;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TB_PopVariation;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TB_TotalMoney;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TB_Incomes;

	UPROPERTY(meta = (BindWidget))
		class UCanvasPanel* CanvasPanel;

	class AMainGameState* GameState;

	class UUI_BuildingInfos* BuildingInfosWidget;



	
};
