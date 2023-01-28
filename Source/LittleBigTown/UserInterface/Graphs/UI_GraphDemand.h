// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LittleBigTown/Core/Enums.h"

#include "UI_GraphDemand.generated.h"

UCLASS()
class LITTLEBIGTOWN_API UUI_GraphDemand : public UUserWidget
{
	GENERATED_BODY()

public :

	void Display(TEnumAsByte <EWealthLevels> WealthLevel = EWealthLevels::Poor);

	void NativeConstruct() override;

	void Hide();
	void UpdateFrequencies();

	UFUNCTION()
		void UpdateNewHour(int32 NewHour);

private : 

	void ReverseProgressBar(bool Reverse, class UProgressBar* ProgressBar);

	UFUNCTION()
		void PoorButtonClicked();

	UFUNCTION()
		void MiddleButtonClicked();

	UFUNCTION()
		void RichButtonClicked();


protected : 

	class AMainGameState* GameState;

	TEnumAsByte <EWealthLevels> LastWealthLevel;

	UPROPERTY(Meta = (BindWidget))
		class UHorizontalBox* HB_ProgressBars;


	// ----------------------------------------------- BUTTONS -----------------------------------------------

	UPROPERTY(Meta = (BindWidget))
		class UButton* Button_Poor;

	UPROPERTY(Meta = (BindWidget))
		class UButton* Button_Middle;

	UPROPERTY(Meta = (BindWidget))
		class UButton* Button_Rich;

	/*
	// ----------------------------------------------- PROGRESS BARS -----------------------------------------------

	UPROPERTY(Meta = (BindWidget))
		class UProgressBar* PB_Industry;

	UPROPERTY(Meta = (BindWidget))
		class UProgressBar* PB_Finance;

	UPROPERTY(Meta = (BindWidget))
		class UProgressBar* PB_Science;

	UPROPERTY(Meta = (BindWidget))
		class UProgressBar* PB_Tourism;

	UPROPERTY(Meta = (BindWidget))
		class UProgressBar* PB_Crime;

	UPROPERTY(Meta = (BindWidget))
		class UProgressBar* PB_Military;

	UPROPERTY(Meta = (BindWidget))
		class UProgressBar* PB_Spiritual;
		*/

};
