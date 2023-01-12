// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
// struct residential infos definition
#include "LittleBigTown/Core/Structs.h"

#include "UI_BuildingInfos.generated.h"

// FText MakeProportionText(int32 a, int32 b);


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

	// Function to overload
	// Residential Version
	UFUNCTION()
		void UpdateAndDisplayInfos(const struct FBuildingInfosBase& BaseInfos, const struct FResidentialBuildingInfos& ResInfos);
		
private : 

	void UpdateAndDisplayBaseInfos(const struct FBuildingInfosBase& BaseInfos);

protected :

	void ResetAllComponents();

	TWeakObjectPtr <class AResidentialBuilding> CurrResBuilding;

	UPROPERTY(meta = (BindWidget))
		class UVerticalBox* VerticalBox;


	// --------------------------------------		TEXTBLOCKS		--------------------------------------


	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TB_Name;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TB_Wealth;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TB_Description;

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
