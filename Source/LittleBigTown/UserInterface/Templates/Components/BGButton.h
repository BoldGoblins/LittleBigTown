// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "LittleBigTown/UserInterface/Templates/BGComponentWidget.h"

#include "BGButton.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBGButtonClickedSignature, UBGButton*, Button);


UCLASS()
class LITTLEBIGTOWN_API UBGButton : public UBGComponentWidget
{
	GENERATED_BODY()

public : 

	void NativePreConstruct() override;

	void NativeConstruct() override;

	FOnBGButtonClickedSignature OnBGButtonClicked;

	bool IsClicked() const { return bButtonClicked; }

	bool IsDisabled() const { return bButtonDisabled; }

	UFUNCTION()
		void OnButtonClicked();

	UFUNCTION(BlueprintCallable)
		void SetButtonClicked(bool Clicked);

	UFUNCTION(BlueprintCallable)
		void SetButtonDisabled(bool Disabled);

	UButton* GetButton() const { return Button; }

protected : 

	UPROPERTY(Meta = (BindWidget))
		UButton* Button;

	UPROPERTY(Meta = (BindWidgetOptional))
		class UTextBlock* TextBlock;

	UPROPERTY(EditAnywhere, Category = "Text")
		FText ButtonText;

	UPROPERTY(EditAnywhere, Category = "Text")
		FSlateFontInfo Font;

	UPROPERTY(EditAnywhere, Category = "Style")
		struct FButtonStyle BasicStyle;

	UPROPERTY(EditAnywhere, Category = "Style")
		struct FButtonStyle OnClickedStyle;

	// Si le Button peut être sélectionné (mis en subrillance une fois cliqué)
	UPROPERTY(EditAnywhere, Category = "Settings")
		bool bCanBeSelected { false };

	UPROPERTY(BlueprintReadOnly)
		bool bButtonClicked{ false };

	UPROPERTY(BlueprintReadOnly)
		bool bButtonDisabled{ false };
	
};
