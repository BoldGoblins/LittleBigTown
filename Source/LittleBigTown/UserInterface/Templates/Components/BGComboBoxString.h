// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LittleBigTown/UserInterface/Templates/BGComponentWidget.h"
#include "BGComboBoxString.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnBGComboBoxStringChangedSignature, UBGComboBoxString*, ComboBox, const FString&, Selection, int32, IndexSelected);

UCLASS()
class LITTLEBIGTOWN_API UBGComboBoxString : public UBGComponentWidget
{
	GENERATED_BODY()

public : 

	void NativeConstruct() override;

	FOnBGComboBoxStringChangedSignature OnBGComboBoxStringChanged;

	UWidget* GetWrappedWidget() override { return Cast <UWidget> (ComboBox); }

	void Populate(const TArray <FString>& Options, bool bDefaultOption = false);

	template <class TEXT>
	void Populate(const TArray <TEXT>& Options, bool bDefaultOption = false)
	{
		bCanBroadcastSelectionChanged = false;

		ComboBox->ClearOptions();

		if (bDefaultOption)
			ComboBox->AddOption(DefaultOption.ToString());

		for (const auto& Option : Options)
			ComboBox->AddOption(Option.ToString());

		ComboBox->RefreshOptions();
		ComboBox->SetSelectedIndex(0);

		bCanBroadcastSelectionChanged = true;
	}

private : 

	UFUNCTION()
		void BroadcastSelectionChangedDelegate(FString Option, ESelectInfo::Type SelectionType);

	// Set to false to prevent OnSelectionChanged Delegate to be Broadcast
	// Comme fonction Populate change la Selection, permet d'éviter que les fonctions 
	// ne se rappellent à l'infini
	bool bCanBroadcastSelectionChanged { true };

protected : 

	UPROPERTY(Meta = (BindWidget))
		class UComboBoxString* ComboBox;

	UPROPERTY(EditAnywhere, Category = "Content")
		FText DefaultOption;
	
};
