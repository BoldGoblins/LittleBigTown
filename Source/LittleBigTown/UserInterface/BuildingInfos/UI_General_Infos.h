// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
// #include "Blueprint/UserWidget.h"
#include "LittleBigTown/UserInterface/Templates/BGLayerWidget.h"
// struct residential infos definition
#include "LittleBigTown/Core/Structs.h"

#include "UI_General_Infos.generated.h"


UCLASS()
class LITTLEBIGTOWN_API UUI_General_Infos : public UBGLayerWidget
{
	GENERATED_BODY()

public:
	
	// Building ptr doit avoir été check par le Widget Parent avant transmission.
	void SetInformations(class ABuilding * Building, bool bNewDisplay = true) override;

	void Reset() override;

private:

	void SetBaseInfos(const struct FBuildingInfosBase& BaseInfos, bool bNewDisplay);

	void SetResidentialInfos(const struct FResidentialBuildingInfos& ResInfos);

protected:


	UPROPERTY(meta = (BindWidget))
		class UVerticalBox* VerticalBox;


	// --------------------------------------		TEXTBLOCKS		--------------------------------------


	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TB_Wealth;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TB_Incomes;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TB_Outgoings;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TB_Level;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TB_OccupationCount;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TB_OccupationMax;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TB_Satisfaction;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TB_Description;


	// --------------------------------------		HORIZONTAL BOXES		--------------------------------------


	UPROPERTY(meta = (BindWidget))
		class UHorizontalBox* HB_Incomes;

	UPROPERTY(meta = (BindWidget))
		class UHorizontalBox* HB_Outgoings;

	UPROPERTY(meta = (BindWidget))
		class UHorizontalBox* HB_Level;

	UPROPERTY(meta = (BindWidget))
		class UHorizontalBox* HB_Occupation;

	UPROPERTY(meta = (BindWidget))
		class UHorizontalBox* HB_Satisfaction;


	// --------------------------------------		PROGRESS BARS		--------------------------------------


	UPROPERTY(meta = (BindWidget))
		class UProgressBar* PB_Level;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* PB_Occupation;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* PB_Satisfaction;
};