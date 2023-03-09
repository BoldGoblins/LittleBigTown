// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LittleBigTown/UserInterface/Templates/BGUserWidget.h"
// struct residential infos definition
#include "LittleBigTown/Core/Structs.h"
#include "LittleBigTown/UserInterface/BuildingInfos/UI_HappinessInfos.h"

#include "UI_BuildingInfos.generated.h"

// FText MakeProportionText(int32 a, int32 b);


UCLASS()
class LITTLEBIGTOWN_API UUI_BuildingInfos : public UBGUserWidget
{
	GENERATED_BODY()


public : 

	void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
		void NewDisplayBuildingInfos(class ABuilding* Building);

	void Hide() override;

	void Update() override;

	// Fonction check l'index du Button dans la Box pour définir l'index du Switcher
	// Button et Child du Widget Switcher doivent être inséré dans le même ordre
	void ButtonClicked(class UBGButton* Button) override;

	// UUI_HappinessInfos* GetHappinessInfos() const { return HappinessInfosWidget;  }

	const TWeakObjectPtr <ABuilding>& GetBuildingDisplayed() const { return DisplayedBuilding; }

private :

	TArray <class UBGLayerWidget*> Layers {};

	TWeakObjectPtr <class ABuilding> DisplayedBuilding;

	TWeakObjectPtr <class ABGMainPlayerController> MainPlayerController { nullptr };

	// class AMainPlayerController* MainPlayerController { nullptr };

protected:


	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
		class UHorizontalBox* ButtonBox;


	// --------------------------------------		TEXTBLOCKS		--------------------------------------


	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TB_Name;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TB_Owner;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TB_District;

};
