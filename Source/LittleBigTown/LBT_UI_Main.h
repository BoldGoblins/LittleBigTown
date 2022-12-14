// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
// #include "LBTGameTimeIF.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "MainGameState.h"
#include "Kismet/GameplayStatics.h"

#include "LBT_UI_Main.generated.h"

UCLASS()
class LITTLEBIGTOWN_API ULBT_UI_Main : public UUserWidget
{
	GENERATED_BODY()

public : 

	void GetClockUpdate(const FDateTime& GameClock, float MonthPercentage);

	FString DateFormat(const FDateTime& GameClock);

	void NativeConstruct() override;

	void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

protected :

	UPROPERTY(meta = (BindWidget))
		UProgressBar* ProgressBar;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* TextBlock;

	AMainGameState* GameState;



	
};
